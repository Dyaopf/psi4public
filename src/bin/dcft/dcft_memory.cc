#include "dcft.h"
#include "defines.h"
#include <vector>
#include <liboptions/liboptions.h>
#include <libpsio/psio.hpp>
#include <libtrans/integraltransform.h>
#include <libmints/wavefunction.h>
#include <libmints/molecule.h>
#include <libtrans/mospace.h>
#include <libdpd/dpd.h>
#include <libiwl/iwl.hpp>

using namespace boost;

namespace psi{ namespace dcft{

/**
 * Reads the orbital information that can be determined before the SCF procedure
 * and initializes SO matrices.
 */
void
DCFTSolver::init()
{
    nso_        = reference_wavefunction_->nso();
    nirrep_     = reference_wavefunction_->nirrep();
    nmo_        = reference_wavefunction_->nmo();
    enuc_       = Process::environment.molecule()->nuclear_repulsion_energy();
    scf_energy_ = reference_wavefunction_->reference_energy();
    ntriso_     = nso_ * (nso_ + 1) / 2;
    for(int h = 0; h < nirrep_; ++h){
        soccpi_[h] = reference_wavefunction_->soccpi()[h];
        doccpi_[h] = reference_wavefunction_->doccpi()[h];
        frzcpi_[h] = reference_wavefunction_->frzcpi()[h];
        frzvpi_[h] = reference_wavefunction_->frzvpi()[h];
        nmopi_[h]  = reference_wavefunction_->nmopi()[h];
        nsopi_[h]  = reference_wavefunction_->nsopi()[h];
    }

    naoccpi_ = new int[nirrep_];
    nboccpi_ = new int[nirrep_];
    navirpi_ = new int[nirrep_];
    nbvirpi_ = new int[nirrep_];
    nalpha_  = nbeta_ =  navir_ = nbvir_ = 0;
    for(int h = 0; h < nirrep_; ++h){
        naoccpi_[h] = doccpi_[h] + soccpi_[h];
        nboccpi_[h] = doccpi_[h];
        navirpi_[h] = nmopi_[h] - doccpi_[h] - soccpi_[h] - frzvpi_[h];
        nbvirpi_[h] = nmopi_[h] - doccpi_[h] - frzvpi_[h];
        for(int n = 0; n < naoccpi_[h]; ++n) ++nalpha_;
        for(int n = 0; n < nboccpi_[h]; ++n) ++nbeta_;
        for(int n = 0; n < navirpi_[h]; ++n) ++navir_;
        for(int n = 0; n < nbvirpi_[h]; ++n) ++nbvir_;
    }

    aocc_c_      = boost::shared_ptr<Matrix>(new Matrix("Alpha Occupied MO Coefficients", nirrep_, nsopi_, naoccpi_));
    bocc_c_      = boost::shared_ptr<Matrix>(new Matrix("Beta Occupied MO Coefficients", nirrep_, nsopi_, nboccpi_));
    avir_c_      = boost::shared_ptr<Matrix>(new Matrix("Alpha Virtual MO Coefficients", nirrep_, nsopi_, navirpi_));
    bvir_c_      = boost::shared_ptr<Matrix>(new Matrix("Beta Virtual MO Coefficients", nirrep_, nsopi_, nbvirpi_));
    scf_error_a_ = boost::shared_ptr<Matrix>(new Matrix("Alpha SCF Error Vector", nirrep_, nsopi_, nsopi_));
    scf_error_b_ = boost::shared_ptr<Matrix>(new Matrix("Beta SCF Error Vector", nirrep_, nsopi_, nsopi_));
    Fa_          = boost::shared_ptr<Matrix>(new Matrix("Alpha Fock Matrix", nirrep_, nsopi_, nsopi_));
    Fb_          = boost::shared_ptr<Matrix>(new Matrix("Beta Fock Matrix", nirrep_, nsopi_, nsopi_));
    Ca_          = boost::shared_ptr<Matrix>(new Matrix("Alpha MO Coefficients", nirrep_, nsopi_, nsopi_));
    Cb_          = boost::shared_ptr<Matrix>(new Matrix("Beta MO Coefficients", nirrep_, nsopi_, nsopi_));
    old_ca_      = boost::shared_ptr<Matrix>(new Matrix("Old Alpha MO Coefficients", nirrep_, nsopi_, nsopi_));
    old_cb_      = boost::shared_ptr<Matrix>(new Matrix("Old Beta MO Coefficients", nirrep_, nsopi_, nsopi_));
    kappa_a_     = boost::shared_ptr<Matrix>(new Matrix("Alpha Kappa Matrix", nirrep_, nsopi_, nsopi_));
    kappa_b_     = boost::shared_ptr<Matrix>(new Matrix("Beta Kappa Matrix", nirrep_, nsopi_, nsopi_));
    g_tau_a_     = boost::shared_ptr<Matrix>(new Matrix("Alpha External Potential Matrix", nirrep_, nsopi_, nsopi_));
    g_tau_b_     = boost::shared_ptr<Matrix>(new Matrix("Beta External Potential Matrix", nirrep_, nsopi_, nsopi_));
    ao_s_        = boost::shared_ptr<Matrix>(new Matrix("SO Basis Overlap Integrals", nirrep_, nsopi_, nsopi_));
    so_h_        = boost::shared_ptr<Matrix>(new Matrix("SO basis one-electron integrals", nirrep_, nsopi_, nsopi_));
    s_half_inv_  = boost::shared_ptr<Matrix>(new Matrix("SO Basis Inverse Square Root Overlap Matrix", nirrep_, nsopi_, nsopi_));
    epsilon_a_   = boost::shared_ptr<Vector>(new Vector(nirrep_, nsopi_));
    epsilon_b_   = boost::shared_ptr<Vector>(new Vector(nirrep_, nsopi_));

    a_tau_    = new double**[nirrep_];
    b_tau_    = new double**[nirrep_];
    for(int h = 0; h < nirrep_; ++h){
        a_tau_[h] = block_matrix(nsopi_[h], nsopi_[h]);
        b_tau_[h] = block_matrix(nsopi_[h], nsopi_[h]);
    }
    

    // Store the AO overlap matrix
    double *sArray = new double[ntriso_];
    IWL::read_one(psio_.get(), PSIF_OEI, PSIF_SO_S, sArray, ntriso_, 0, 0, outfile);
    ao_s_->set(sArray);
    delete [] sArray;

    // Form S^(-1/2) matrix
    Matrix eigvec(nirrep_, nsopi_, nsopi_);
    Matrix eigtemp(nirrep_, nsopi_, nsopi_);
    Matrix eigtemp2(nirrep_, nsopi_, nsopi_);
    Vector eigval(nirrep_, nsopi_);
    ao_s_->diagonalize(eigvec, eigval);
    // Convert the eigenvales to 1/sqrt(eigenvalues)
    for (int h=0; h < nirrep_; ++h) {
        for (int i=0; i < nsopi_[h]; ++i) {
            double scale = 1.0 / sqrt(eigval.get(h, i));
            eigval.set(h, i, scale);
        }
    }
    eigtemp2.set_diagonal(eigval);
    eigtemp.gemm(false, true, 1.0, eigtemp2, eigvec, 0.0);
    s_half_inv_->gemm(false, false, 1.0, eigvec, eigtemp, 0.0);
}


/**
 * Frees up the memory sequestered by the init_moinfo() and read_checkpoint() routines.
 */
void
DCFTSolver::finalize()
{
    psio_->close(PSIF_DCFT_DPD, 1);
    chkpt_.reset();
    delete _ints;

    aocc_c_.reset();
    bocc_c_.reset();
    avir_c_.reset();
    bvir_c_.reset();
    scf_error_a_.reset();
    scf_error_b_.reset();
    Fa_.reset();
    Fb_.reset();
    old_ca_.reset();
    old_cb_.reset();
    kappa_a_.reset();
    kappa_b_.reset();
    g_tau_a_.reset();
    g_tau_b_.reset();
    ao_s_.reset();
    so_h_.reset();
    s_half_inv_.reset();

    for(int h = 0; h < nirrep_; ++h){
        free_block(a_tau_[h]);
        free_block(b_tau_[h]);
    }
    delete [] a_tau_;
    delete [] b_tau_;
    delete [] naoccpi_;
    delete [] nboccpi_;
    delete [] navirpi_;
    delete [] nbvirpi_;
}

}}//Namespaces
