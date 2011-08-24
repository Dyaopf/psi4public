#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <utility>
#include <string>
#include <cstring>

#include <psifiles.h>
#include <physconst.h>
#include <libciomr/libciomr.h>
#include <libpsio/psio.hpp>
#include <libchkpt/chkpt.hpp>
#include <libiwl/iwl.hpp>
#include <libqt/qt.h>

#include <libmints/mints.h>
#include <libfunctional/superfunctional.h>
#include <lib3index/3index.h>
#include "ks.h"
#include "dft.h"
#include "integralfunctors.h"
#include "omegafunctors.h"

using namespace std;
using namespace psi;
using namespace psi::functional;
using namespace boost;

namespace psi { namespace scf {

KS::KS(Options & options, boost::shared_ptr<PSIO> psio) :
    options_(options), psio_(psio)
{
    common_init();
}
KS::~KS()
{
}
void KS::common_init()
{
    // Take the molecule from the environment
    molecule_ = Process::environment.molecule();

    // Load in the basis set
    boost::shared_ptr<BasisSetParser> parser(new Gaussian94BasisSetParser());
    basisset_ = BasisSet::construct(parser, molecule_, "BASIS");
    boost::shared_ptr<IntegralFactory> fact(new IntegralFactory(basisset_,basisset_,basisset_,basisset_));
    sobasisset_ = boost::shared_ptr<SOBasisSet>(new SOBasisSet(basisset_, fact));

    //Build the superfunctional
    int block_size = options_.get_int("DFT_BLOCK_MAX");
    functional_ = SuperFunctional::createSuperFunctional(options_.get_str("DFT_FUNCTIONAL"),block_size,1);

    // Temporary print, to make sure we're in the right spot
    fprintf(outfile,"\n  Selected Functional is %s.\n\n",functional_->getName().c_str());

    //Grab the properties object for this basis
    if (functional_->isRangeCorrected()) {

        if (options_["DFT_OMEGA"].has_changed()) {
            functional_->setOmega(options_.get_double("DFT_OMEGA"));
        }

        std::vector<boost::shared_ptr<TwoBodyAOInt> > aoint;
        for (int i=0; i<Communicator::world->nthread(); ++i)
            aoint.push_back(boost::shared_ptr<TwoBodyAOInt>(fact->erf_eri(functional_->getOmega())));
        omega_eri_ = boost::shared_ptr<TwoBodySOInt>(new TwoBodySOInt(aoint, fact));
    }
}
RKS::RKS(Options & options, boost::shared_ptr<PSIO> psio, boost::shared_ptr<Chkpt> chkpt) :
    RHF(options, psio, chkpt), KS(options,psio)
{
    common_init();
}
RKS::RKS(Options & options, boost::shared_ptr<PSIO> psio) :
    RHF(options, psio), KS(options,psio)
{
    common_init();
}
void RKS::common_init()
{
    potential_ = boost::shared_ptr<RKSPotential>(new RKSPotential(functional_,KS::molecule_,KS::basisset_,KS::options_));
    wK_ = factory_->create_shared_matrix("wKa (Long-Range Hartree-Fock Exchange)");
}
RKS::~RKS()
{
}
void RKS::form_V()
{
    boost::shared_ptr<Dimension> doccpi(new Dimension(nirrep_));
    for (int h = 0; h < nirrep_; h++) {
        (*doccpi)[h] = doccpi_[h];
    }

    potential_->buildPotential(D_,Ca_,doccpi);
    V_ = potential_->V_USO(); 
}
void RKS::form_G()
{
    form_V();
    if (functional_->isRangeCorrected()) {
        Omega_K_Functor k_builder(functional_->getOmega(), wK_, D_, Ca_, nalphapi_);
        process_omega_tei(k_builder);
    }
    if (!functional_->isHybrid()) {
        // This will build J (stored in G)
        J_Functor j_builder(G_, D_);
        process_tei(j_builder);
        J_->copy(G_);

        G_->scale(1.0);
        G_->add(V_);
        G_->subtract(wK_);

    } else {
        // This will build J (stored in G) and K
        J_K_Functor jk_builder(G_, K_, D_, Ca_, nalphapi_);
        process_tei(jk_builder);
        J_->copy(G_);

        double alpha = functional_->getExactExchange();
        double beta = 1.0 - alpha;
        G_->scale(1.0);
        K_->scale(alpha);
        G_->subtract(K_);
        K_->scale(1.0/alpha);
        G_->add(V_);
        wK_->scale(beta);
        G_->subtract(wK_);
        wK_->scale(1.0/beta);
    }

    if (debug_ > 2) {
        J_->print();
        K_->print();
        wK_->print();
        V_->print();
    }
}
double RKS::compute_E()
{
    // E_DFT = 2.0 D*H + 2.0 D*J - \alpha D*K + E_xc
    double one_electron_E = 2.0*D_->vector_dot(H_);
    double coulomb_E = D_->vector_dot(J_);
    
    double XC_E = potential_->quadrature_value("FUNCTIONAL");
    double exchange_E = 0.0;
    double alpha = functional_->getExactExchange();
    double beta = 1.0 - alpha;
    if (functional_->isHybrid()) {
        exchange_E -= alpha*Da_->vector_dot(K_);
    }
    if (functional_->isRangeCorrected()) {
        exchange_E -=  beta*Da_->vector_dot(wK_);
    }

    double Etotal = 0.0;
    Etotal += nuclearrep_;
    Etotal += one_electron_E;
    Etotal += coulomb_E;
    Etotal += exchange_E;
    Etotal += XC_E; 
    double dashD_E = 0.0;
    if (functional_->isDashD()) {
        dashD_E = functional_->getDashD()->computeEnergy(HF::molecule_);
    }
    Etotal += dashD_E;

    if (debug_ > 2) {
        fprintf(outfile, "Nuclear Repulsion Energy = %24.14f\n", nuclearrep_);
        fprintf(outfile, "One-Electron Energy =      %24.14f\n", one_electron_E);
        fprintf(outfile, "Coulomb Energy =           %24.14f\n", coulomb_E);
        fprintf(outfile, "Hybrid Exchange Energy =   %24.14f\n", exchange_E);
        fprintf(outfile, "XC Functional Energy =     %24.14f\n", XC_E); 
        fprintf(outfile, "-D Energy =                %24.14f\n", dashD_E);
    }

    return Etotal;
}
UKS::UKS(Options & options, boost::shared_ptr<PSIO> psio, boost::shared_ptr<Chkpt> chkpt) :
    UHF(options, psio, chkpt), KS(options,psio)
{
    common_init();
}
UKS::UKS(Options & options, boost::shared_ptr<PSIO> psio) :
    UHF(options, psio), KS(options,psio)
{
    common_init();
}
void UKS::common_init()
{
    potential_ = boost::shared_ptr<UKSPotential>(new UKSPotential(functional_,KS::molecule_,KS::basisset_,KS::options_));
    wKa_ = factory_->create_shared_matrix("wKa (Long-Range Hartree-Fock Exchange)");
    wKb_ = factory_->create_shared_matrix("wKb (Long-Range Hartree-Fock Exchange)");
}
UKS::~UKS()
{
}
void UKS::form_V()
{
    boost::shared_ptr<Dimension> napi(new Dimension(nirrep_));
    boost::shared_ptr<Dimension> nbpi(new Dimension(nirrep_));
    for (int h = 0; h < nirrep_; h++) {
        (*napi)[h] = nalphapi_[h];
        (*nbpi)[h] = nbetapi_[h];
    }

    potential_->buildPotential(Da_,Ca_,napi, 
                               Db_,Cb_,nbpi);
    Va_ = potential_->Va_USO(); 
    Vb_ = potential_->Vb_USO(); 
}
void UKS::form_G()
{
    timer_on("Form V");
    form_V();
    timer_off("Form V");

    if (functional_->isRangeCorrected()) {
        Omega_Ka_Kb_Functor k_builder(functional_->getOmega(),wKa_,wKb_,Da_,Db_,Ca_,Cb_,nalphapi_,nbetapi_);
        process_omega_tei<Omega_Ka_Kb_Functor>(k_builder);
    }
    if (!functional_->isHybrid()) {
        // This will build J (stored in G)
        boost::shared_ptr<Matrix> Dh(factory_->create_matrix("Dh"));
        Dh->copy(Da_);
        Dh->add(Db_);
        Dh->scale(0.5);

        J_Functor j_builder(Ga_, Dh);
        process_tei<J_Functor>(j_builder);
        J_->copy(Ga_);

        Gb_->copy(Ga_);
        Ga_->add(Va_);
        Gb_->add(Vb_);
        Ga_->subtract(wKa_);
        Gb_->subtract(wKb_);

    } else {
        // This will build J (stored in G) and K
        J_Ka_Kb_Functor jk_builder(Ga_, Ka_, Kb_, Da_, Db_, Ca_, Cb_, nalphapi_, nbetapi_);
        process_tei<J_Ka_Kb_Functor>(jk_builder);
        J_->copy(Ga_);
        Gb_->copy(Ga_);

        double alpha = functional_->getExactExchange();
        double beta = 1.0 - alpha;
        Ka_->scale(alpha);
        Kb_->scale(alpha);
        Ga_->subtract(Ka_);
        Gb_->subtract(Kb_);
        Ka_->scale(1.0/alpha);
        Kb_->scale(1.0/alpha);
        Ga_->add(Va_);
        Gb_->add(Vb_);
        wKa_->scale(beta);
        wKb_->scale(beta);
        Ga_->subtract(wKa_);
        Gb_->subtract(wKb_);
        wKa_->scale(1.0/beta);
        wKb_->scale(1.0/beta);
    }

    if (debug_ > 2) {
        J_->print(outfile);
        Ka_->print(outfile);
        Kb_->print(outfile);
        wKa_->print(outfile);
        wKb_->print(outfile);
        Va_->print();
        Vb_->print();
    }
}
double UKS::compute_E()
{
    // E_DFT = 2.0 D*H + D*J - \alpha D*K + E_xc
    double one_electron_E = Da_->vector_dot(H_);
    one_electron_E += Db_->vector_dot(H_);
    double coulomb_E = Da_->vector_dot(J_);
    coulomb_E += Db_->vector_dot(J_);

    double XC_E = potential_->quadrature_value("FUNCTIONAL");
    double exchange_E = 0.0;
    double alpha = functional_->getExactExchange();
    double beta = 1.0 - alpha;
    if (functional_->isHybrid()) {
        exchange_E -= alpha*Da_->vector_dot(Ka_);
        exchange_E -= alpha*Db_->vector_dot(Kb_);
    }
    if (functional_->isRangeCorrected()) {
        exchange_E -=  beta*Da_->vector_dot(wKa_);
        exchange_E -=  beta*Db_->vector_dot(wKb_);
    }

    double Etotal = 0.0;
    Etotal += nuclearrep_;
    Etotal += one_electron_E;
    Etotal += 0.5 * coulomb_E;
    Etotal += 0.5 * exchange_E;
    Etotal += XC_E; 
    double dashD_E=0.0;
    if (functional_->isDashD()) {
        dashD_E = functional_->getDashD()->computeEnergy(HF::molecule_);
    }
    Etotal += dashD_E;

    if (debug_ > 2) {
        fprintf(outfile, "Nuclear Repulsion Energy = %24.14f\n", nuclearrep_);
        fprintf(outfile, "One-Electron Energy =      %24.14f\n", one_electron_E);
        fprintf(outfile, "Coulomb Energy =           %24.14f\n", 0.5 * coulomb_E);
        fprintf(outfile, "Hybrid Exchange Energy =   %24.14f\n", 0.5 * exchange_E);
        fprintf(outfile, "XC Functional Energy =     %24.14f\n", XC_E); 
        fprintf(outfile, "-D Energy =                %24.14f\n", dashD_E);
    }

    return Etotal;
}


}}
