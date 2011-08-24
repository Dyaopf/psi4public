#include <libmints/mints.h>
#include <libqt/qt.h>
#include <math.h>
#include "points.h"
#include "cubature.h"
#include "psiconfig.h"

namespace psi {

RKSFunctions::RKSFunctions(boost::shared_ptr<BasisSet> primary, int max_points, int max_functions) :
    PointFunctions(primary,max_points,max_functions)
{
    set_ansatz(0);
}
RKSFunctions::~RKSFunctions()
{
}
void RKSFunctions::build_temps()
{
    temp_ = boost::shared_ptr<Matrix>(new Matrix("Temp",max_points_,max_functions_));
    D_local_ = boost::shared_ptr<Matrix>(new Matrix("Dlocal",max_functions_,max_functions_));

    if (ansatz_ >= 2) {
        int nocc = Cocc_AO_->colspi()[0];
        C_local_ = boost::shared_ptr<Matrix>(new Matrix("Clocal",max_functions_,nocc));
        meta_temp_ = boost::shared_ptr<Matrix>(new Matrix("Meta Temp", max_points_,nocc));
    }
}
void RKSFunctions::set_ansatz(int ansatz) 
{
    ansatz_ = ansatz;

    property_values_.clear();

    if (ansatz_ >= 1) {
        set_derivative(1);
    } else {
        set_derivative(0);
    }

    if (ansatz_ >= 0) {
        property_values_["RHO_A"] = boost::shared_ptr<Vector>(new Vector("RHO_A", max_points_)); 
        property_values_["RHO_B"] = property_values_["RHO_A"]; 
    } 

    if (ansatz_ >= 1) {
        property_values_["RHO_AX"] = boost::shared_ptr<Vector>(new Vector("RHO_AX", max_points_)); 
        property_values_["RHO_AY"] = boost::shared_ptr<Vector>(new Vector("RHO_AY", max_points_)); 
        property_values_["RHO_AZ"] = boost::shared_ptr<Vector>(new Vector("RHO_AZ", max_points_)); 
        property_values_["RHO_BX"] = property_values_["RHO_AX"]; 
        property_values_["RHO_BY"] = property_values_["RHO_AY"]; 
        property_values_["RHO_BZ"] = property_values_["RHO_AZ"]; 
        property_values_["GAMMA_AA"] = boost::shared_ptr<Vector>(new Vector("GAMMA_AA", max_points_)); 
        property_values_["GAMMA_AB"] = property_values_["GAMMA_AA"]; 
        property_values_["GAMMA_BB"] = property_values_["GAMMA_AA"]; 
    } 

    if (ansatz_ >= 2) {
        property_values_["TAU_A"] = boost::shared_ptr<Vector>(new Vector("TAU_A", max_points_)); 
        property_values_["TAU_B"] = property_values_["TAU_A"]; 
    } 
}
void RKSFunctions::reset_pointers(boost::shared_ptr<Matrix> D_AO, boost::shared_ptr<Matrix> Cocc_AO) 
{
    D_AO_ = D_AO;
    Cocc_AO_ = Cocc_AO_;
    build_temps();
}
boost::shared_ptr<Vector> RKSFunctions::property_value(const std::string& key)
{
    return property_values_[key]; 
}
void RKSFunctions::computeProperties(boost::shared_ptr<BlockOPoints> block)
{
    // => Build basis function values <= //
    PointFunctions::computePoints(block);

    // => Global information <= //
    int npoints = block->npoints();
    const std::vector<int>& function_map = block->functions_local_to_global();
    int nglobal = max_functions_; 
    int nlocal  = function_map.size();

    double** Tp = temp_->pointer();

    // => Build local D matrix <= // 
    double** Dp = D_AO_->pointer();
    double** D2p = D_local_->pointer();

    for (int ml = 0; ml < nlocal; ml++) {
        int mg = function_map[ml];
        for (int nl = 0; nl <= ml; nl++) {
            int ng = function_map[nl];
            
            double Dval = Dp[mg][ng];

            D2p[ml][nl] = Dval; 
            D2p[nl][ml] = Dval; 
        } 
    }

    // => Build LSDA quantities <= //
    double** phip = basis_values_["PHI"]->pointer(); 
    double* rhoap = property_values_["RHO_A"]->pointer();

    C_DGEMM('N','N',npoints,nlocal,nlocal,1.0,phip[0],nglobal,D2p[0],nglobal,0.0,Tp[0],nglobal);
    for (int P = 0; P < npoints; P++) {
        rhoap[P] = C_DDOT(nlocal,phip[P],1,Tp[P],1);
    } 

    // => Build GGA quantities <= //
    if (ansatz_ >= 1) {    

        double** phixp = basis_values_["PHI_X"]->pointer(); 
        double** phiyp = basis_values_["PHI_Y"]->pointer(); 
        double** phizp = basis_values_["PHI_Z"]->pointer(); 
        double* rhoaxp = property_values_["RHO_AX"]->pointer();
        double* rhoayp = property_values_["RHO_AY"]->pointer();
        double* rhoazp = property_values_["RHO_AZ"]->pointer();
        double* gammaaap = property_values_["GAMMA_AA"]->pointer();

        for (int P = 0; P < npoints; P++) {
            double rho_x = 2.0 * C_DDOT(nlocal,phixp[P],1,Tp[P],1);
            double rho_y = 2.0 * C_DDOT(nlocal,phiyp[P],1,Tp[P],1);
            double rho_z = 2.0 * C_DDOT(nlocal,phizp[P],1,Tp[P],1);
            rhoaxp[P] = rho_x;
            rhoayp[P] = rho_y;
            rhoazp[P] = rho_z;
            gammaaap[P] = rho_x * rho_x + rho_y * rho_y + rho_z * rho_z;
        } 
    }

    // => Build Meta quantities <= //
    if (ansatz_ >= 2) {    
        
        // => Build local C matrix <= //
        int na = Cocc_AO_->colspi()[0]; 
        double** Cp = Cocc_AO_->pointer();
        double** C2p = C_local_->pointer();
        double** TCp = meta_temp_->pointer();

        for (int mlocal = 0; mlocal < nlocal; mlocal++) {
            int mglobal = function_map[mlocal];
            ::memcpy(static_cast<void*>(C2p[mlocal]), static_cast<void*>(Cp[mglobal]), na * sizeof(double));
        }

        // => Build KE density A(N^2) <= //
        double** phixp = basis_values_["PHI_X"]->pointer(); 
        double** phiyp = basis_values_["PHI_Y"]->pointer(); 
        double** phizp = basis_values_["PHI_Z"]->pointer(); 
        double* tauap = property_values_["TAU_A"]->pointer();

        // \nabla x
        C_DGEMM('T','N',npoints,na,nlocal,1.0,phixp[0],nlocal,C2p[0],na,0.0,TCp[0],na);
        for (int P = 0; P < npoints; P++) {
            tauap[P] = C_DDOT(na,TCp[P],1,TCp[P],1);
        }        

        // \nabla y
        C_DGEMM('T','N',npoints,na,nlocal,1.0,phiyp[0],nlocal,C2p[0],na,0.0,TCp[0],na);
        for (int P = 0; P < npoints; P++) {
            tauap[P] += C_DDOT(na,TCp[P],1,TCp[P],1);
        }        

        // \nabla z
        C_DGEMM('T','N',npoints,na,nlocal,1.0,phizp[0],nlocal,C2p[0],na,0.0,TCp[0],na);
        for (int P = 0; P < npoints; P++) {
            tauap[P] += C_DDOT(na,TCp[P],1,TCp[P],1);
        }        
    }
}
void RKSFunctions::print(FILE* out, int print) const
{
    std::string ans;
    if (ansatz_ == 0) {
        ans = "LSDA";
    } else if (ansatz_ == 1) {
        ans = "GGA";
    } else if (ansatz_ == 2) {
        ans = "Meta-GGA";
    }

    fprintf(out, "   => RKSFunctions: %s Ansatz <=\n\n", ans.c_str()); 

    fprintf(out, "    Property Values:\n");
    for (std::map<std::string, boost::shared_ptr<Vector> >::const_iterator it = property_values_.begin();
        it != property_values_.end(); it++) {
        fprintf(out, "    %s\n", (*it).first.c_str());
        if (print > 3) {
            (*it).second->print();
        }
    }
    fprintf(out,"\n\n");

    PointFunctions::print(out,print);
}

UKSFunctions::UKSFunctions(boost::shared_ptr<BasisSet> primary, int max_points, int max_functions) :
    PointFunctions(primary,max_points, max_functions)
{
    set_ansatz(0);
}
UKSFunctions::~UKSFunctions()
{
}
void UKSFunctions::build_temps()
{
    tempa_ = boost::shared_ptr<Matrix>(new Matrix("Temp",max_points_,max_functions_));
    Da_local_ = boost::shared_ptr<Matrix>(new Matrix("Dlocal",max_functions_,max_functions_));
    tempb_ = boost::shared_ptr<Matrix>(new Matrix("Temp",max_points_,max_functions_));
    Db_local_ = boost::shared_ptr<Matrix>(new Matrix("Dlocal",max_functions_,max_functions_));

    if (ansatz_ >= 2) {
        int nocca = Caocc_AO_->colspi()[0];
        int noccb = Cbocc_AO_->colspi()[0];
        int nocc  = (nocca > noccb ? nocca : noccb);
        Ca_local_ = boost::shared_ptr<Matrix>(new Matrix("Clocal",max_functions_,nocca));
        Cb_local_ = boost::shared_ptr<Matrix>(new Matrix("Clocal",max_functions_,noccb));
        meta_temp_ = boost::shared_ptr<Matrix>(new Matrix("Meta Temp", max_points_,nocc));
    }
}
void UKSFunctions::set_ansatz(int ansatz) 
{
    ansatz_ = ansatz;

    property_values_.clear();

    if (ansatz_ >= 1) {
        set_derivative(1);
    } else {
        set_derivative(0);
    }

    if (ansatz_ >= 0) {
        property_values_["RHO_A"] = boost::shared_ptr<Vector>(new Vector("RHO_A", max_points_)); 
        property_values_["RHO_B"] = boost::shared_ptr<Vector>(new Vector("RHO_B", max_points_));
    } 

    if (ansatz_ >= 1) {
        property_values_["RHO_AX"] = boost::shared_ptr<Vector>(new Vector("RHO_AX", max_points_)); 
        property_values_["RHO_AY"] = boost::shared_ptr<Vector>(new Vector("RHO_AY", max_points_)); 
        property_values_["RHO_AZ"] = boost::shared_ptr<Vector>(new Vector("RHO_AZ", max_points_)); 
        property_values_["RHO_BX"] = boost::shared_ptr<Vector>(new Vector("RHO_BX", max_points_)); 
        property_values_["RHO_BY"] = boost::shared_ptr<Vector>(new Vector("RHO_BY", max_points_)); 
        property_values_["RHO_BZ"] = boost::shared_ptr<Vector>(new Vector("RHO_BZ", max_points_)); 
        property_values_["GAMMA_AA"] = boost::shared_ptr<Vector>(new Vector("GAMMA_AA", max_points_)); 
        property_values_["GAMMA_AB"] = boost::shared_ptr<Vector>(new Vector("GAMMA_AB", max_points_));
        property_values_["GAMMA_BB"] = boost::shared_ptr<Vector>(new Vector("GAMMA_BB", max_points_)); 
    } 

    if (ansatz_ >= 2) {
        property_values_["TAU_A"] = boost::shared_ptr<Vector>(new Vector("TAU_A", max_points_)); 
        property_values_["TAU_B"] = boost::shared_ptr<Vector>(new Vector("TAU_A", max_points_));
    } 
}
void UKSFunctions::reset_pointers(boost::shared_ptr<Matrix> Da_AO, boost::shared_ptr<Matrix> Caocc_AO,
                                  boost::shared_ptr<Matrix> Db_AO, boost::shared_ptr<Matrix> Cbocc_AO) 
{
    Da_AO_ = Da_AO;
    Caocc_AO_ = Caocc_AO_;
    Db_AO_ = Db_AO;
    Cbocc_AO_ = Cbocc_AO_;
    build_temps();
}
boost::shared_ptr<Vector> UKSFunctions::property_value(const std::string& key) 
{
    return property_values_[key]; 
}
void UKSFunctions::computeProperties(boost::shared_ptr<BlockOPoints> block)
{
    // => Build basis function values <= //
    PointFunctions::computePoints(block);

    // => Global information <= //
    int npoints = block->npoints();
    const std::vector<int>& function_map = block->functions_local_to_global();
    int nglobal = max_functions_;
    int nlocal  = function_map.size();

    double** Tap = tempa_->pointer();
    double** Tbp = tempb_->pointer();

    // => Build local D matrix <= // 
    double** Dap = Da_AO_->pointer();
    double** Da2p = Da_local_->pointer();
    double** Dbp = Db_AO_->pointer();
    double** Db2p = Db_local_->pointer();

    for (int ml = 0; ml < nlocal; ml++) {
        int mg = function_map[ml];
        for (int nl = 0; nl <= ml; nl++) {
            int ng = function_map[nl];
            
            double Daval = Dap[mg][ng];
            double Dbval = Dbp[mg][ng];

            Da2p[ml][nl] = Daval; 
            Da2p[nl][ml] = Daval; 
            Db2p[ml][nl] = Dbval; 
            Db2p[nl][ml] = Dbval; 
        } 
    }

    // => Build LSDA quantities <= //
    double** phip = basis_values_["PHI"]->pointer(); 
    double* rhoap = property_values_["RHO_A"]->pointer();
    double* rhobp = property_values_["RHO_B"]->pointer();

    C_DGEMM('N','N',npoints,nlocal,nlocal,1.0,phip[0],nglobal,Da2p[0],nglobal,0.0,Tap[0],nglobal);
    for (int P = 0; P < npoints; P++) {
        rhoap[P] = C_DDOT(nlocal,phip[P],1,Tap[P],1);
    } 

    C_DGEMM('N','N',npoints,nlocal,nlocal,1.0,phip[0],nglobal,Db2p[0],nglobal,0.0,Tbp[0],nglobal);
    for (int P = 0; P < npoints; P++) {
        rhobp[P] = C_DDOT(nlocal,phip[P],1,Tbp[P],1);
    } 

    // => Build GGA quantities <= //
    if (ansatz_ >= 1) {    

        double** phixp = basis_values_["PHI_X"]->pointer(); 
        double** phiyp = basis_values_["PHI_Y"]->pointer(); 
        double** phizp = basis_values_["PHI_Z"]->pointer(); 
        double* rhoaxp = property_values_["RHO_AX"]->pointer();
        double* rhoayp = property_values_["RHO_AY"]->pointer();
        double* rhoazp = property_values_["RHO_AZ"]->pointer();
        double* rhobxp = property_values_["RHO_BX"]->pointer();
        double* rhobyp = property_values_["RHO_BY"]->pointer();
        double* rhobzp = property_values_["RHO_BZ"]->pointer();
        double* gammaaap = property_values_["GAMMA_AA"]->pointer();
        double* gammaabp = property_values_["GAMMA_AB"]->pointer();
        double* gammabbp = property_values_["GAMMA_BB"]->pointer();

        for (int P = 0; P < npoints; P++) {
            double rhoa_x = 2.0 * C_DDOT(nlocal,phixp[P],1,Tap[P],1);
            double rhoa_y = 2.0 * C_DDOT(nlocal,phiyp[P],1,Tap[P],1);
            double rhoa_z = 2.0 * C_DDOT(nlocal,phizp[P],1,Tap[P],1);
            double rhob_x = 2.0 * C_DDOT(nlocal,phixp[P],1,Tbp[P],1);
            double rhob_y = 2.0 * C_DDOT(nlocal,phiyp[P],1,Tbp[P],1);
            double rhob_z = 2.0 * C_DDOT(nlocal,phizp[P],1,Tbp[P],1);
            rhoaxp[P] = rhoa_x;
            rhoayp[P] = rhoa_y;
            rhoazp[P] = rhoa_z;
            rhobxp[P] = rhob_x;
            rhobyp[P] = rhob_y;
            rhobzp[P] = rhob_z;
            gammaaap[P] = rhoa_x * rhoa_x + rhoa_y * rhoa_y + rhoa_z * rhoa_z;
            gammaabp[P] = rhoa_x * rhob_x + rhoa_y * rhob_y + rhoa_z * rhob_z;
            gammabbp[P] = rhob_x * rhob_x + rhob_y * rhob_y + rhob_z * rhob_z;
        } 
    }

    // => Build Meta quantities <= //
    if (ansatz_ >= 2) {    
        
        // => Build local C matrix <= //
        int na = Caocc_AO_->colspi()[0]; 
        int nb = Cbocc_AO_->colspi()[0]; 
        int nc = (na > nb ? na : nb);
        double** Cap = Caocc_AO_->pointer();
        double** Cbp = Cbocc_AO_->pointer();
        double** Ca2p = Ca_local_->pointer();
        double** Cb2p = Cb_local_->pointer();
        double** TCp  = meta_temp_->pointer(); 

        for (int mlocal = 0; mlocal < nlocal; mlocal++) {
            int mglobal = function_map[mlocal];
            ::memcpy(static_cast<void*>(Ca2p[mlocal]), static_cast<void*>(Cap[mglobal]), na * sizeof(double));
            ::memcpy(static_cast<void*>(Cb2p[mlocal]), static_cast<void*>(Cbp[mglobal]), nb * sizeof(double));
        }

        // => Build KE density A(N^2) <= //
        double** phixp = basis_values_["PHI_X"]->pointer(); 
        double** phiyp = basis_values_["PHI_Y"]->pointer(); 
        double** phizp = basis_values_["PHI_Z"]->pointer(); 
        double* tauap = property_values_["TAU_A"]->pointer();
        double* taubp = property_values_["TAU_B"]->pointer();

        // Alpha
        // \nabla x
        C_DGEMM('T','N',npoints,na,nlocal,1.0,phixp[0],nlocal,Ca2p[0],na,0.0,TCp[0],nc);
        for (int P = 0; P < npoints; P++) {
            tauap[P] = C_DDOT(na,TCp[P],1,TCp[P],1);
        }        

        // \nabla y
        C_DGEMM('T','N',npoints,na,nlocal,1.0,phiyp[0],nlocal,Ca2p[0],na,0.0,TCp[0],nc);
        for (int P = 0; P < npoints; P++) {
            tauap[P] += C_DDOT(na,TCp[P],1,TCp[P],1);
        }        

        // \nabla z
        C_DGEMM('T','N',npoints,na,nlocal,1.0,phizp[0],nlocal,Ca2p[0],na,0.0,TCp[0],nc);
        for (int P = 0; P < npoints; P++) {
            tauap[P] += C_DDOT(na,TCp[P],1,TCp[P],1);
        }        
        
        // Beta 
        // \nabla x
        C_DGEMM('T','N',npoints,nb,nlocal,1.0,phixp[0],nlocal,Cb2p[0],nb,0.0,TCp[0],nc);
        for (int P = 0; P < npoints; P++) {
            taubp[P] = C_DDOT(nb,TCp[P],1,TCp[P],1);
        }        

        // \nabla y
        C_DGEMM('T','N',npoints,nb,nlocal,1.0,phiyp[0],nlocal,Cb2p[0],nb,0.0,TCp[0],nc);
        for (int P = 0; P < npoints; P++) {
            taubp[P] += C_DDOT(nb,TCp[P],1,TCp[P],1);
        }        

        // \nabla z
        C_DGEMM('T','N',npoints,nb,nlocal,1.0,phizp[0],nlocal,Cb2p[0],nb,0.0,Tbp[0],nc);
        for (int P = 0; P < npoints; P++) {
            taubp[P] += C_DDOT(nb,TCp[P],1,TCp[P],1);
        }        
    }
}
void UKSFunctions::print(FILE* out, int print) const
{
    std::string ans;
    if (ansatz_ == 0) {
        ans = "LSDA";
    } else if (ansatz_ == 1) {
        ans = "GGA";
    } else if (ansatz_ == 2) {
        ans = "Meta-GGA";
    }

    fprintf(out, "   => UKSFunctions: %s Ansatz <=\n\n", ans.c_str()); 

    fprintf(out, "    Property Values:\n");
    for (std::map<std::string, boost::shared_ptr<Vector> >::const_iterator it = property_values_.begin();
        it != property_values_.end(); it++) {
        fprintf(out, "    %s\n", (*it).first.c_str());
        if (print > 3) {
            (*it).second->print();
        }
    }
    fprintf(out,"\n\n");

    PointFunctions::print(out,print);
}

PointFunctions::PointFunctions(boost::shared_ptr<BasisSet> primary, int max_points, int max_functions) :
    primary_(primary), max_points_(max_points), max_functions_(max_functions), npoints_(0)
{
    build_spherical();
    set_derivative(0);
}
PointFunctions::~PointFunctions()
{
}
void PointFunctions::build_spherical()
{
    boost::shared_ptr<IntegralFactory> fact(new IntegralFactory(primary_,primary_,primary_,primary_));

    for (int L = 0; L <= primary_->max_am(); L++) {
        std::vector<boost::tuple<int,int,double> > comp;
        boost::shared_ptr<SphericalTransformIter> trans(fact->spherical_transform_iter(L));
        for (trans->first(); !trans->is_done();trans->next()) {
            comp.push_back(boost::tuple<int,int,double>(
                trans->pureindex(),
                trans->cartindex(),
                trans->coef()));
        }
        spherical_transforms_.push_back(comp);
    }
}
void PointFunctions::set_derivative(int derivative) 
{
    deriv_ = derivative;
   
    basis_values_.clear();
    basis_temps_.clear();

    int max_am = primary_->max_am();
    int max_cart = (max_am + 1) * (max_am + 2) / 2;
 
    if (deriv_ >= 0) {
        basis_values_["PHI"] = boost::shared_ptr<Matrix> (new Matrix("PHI", max_points_, primary_->nbf()));
        basis_temps_["PHI"] = boost::shared_ptr<Matrix> (new Matrix("PHI", max_points_, max_cart));
    } 

    if (deriv_ >= 1) {
        basis_values_["PHI_X"] = boost::shared_ptr<Matrix> (new Matrix("PHI_X", max_points_, primary_->nbf()));
        basis_values_["PHI_Y"] = boost::shared_ptr<Matrix> (new Matrix("PHI_Y", max_points_, primary_->nbf()));
        basis_values_["PHI_Z"] = boost::shared_ptr<Matrix> (new Matrix("PHI_Z", max_points_, primary_->nbf()));
        basis_temps_["PHI_X"] = boost::shared_ptr<Matrix> (new Matrix("PHI_X", max_points_, max_cart));
        basis_temps_["PHI_Y"] = boost::shared_ptr<Matrix> (new Matrix("PHI_Y", max_points_, max_cart));
        basis_temps_["PHI_Z"] = boost::shared_ptr<Matrix> (new Matrix("PHI_Z", max_points_, max_cart));
    }

    if (deriv_ >= 2) {
        basis_values_["PHI_XX"] = boost::shared_ptr<Matrix> (new Matrix("PHI_XX", max_points_, primary_->nbf()));
        basis_values_["PHI_XY"] = boost::shared_ptr<Matrix> (new Matrix("PHI_XY", max_points_, primary_->nbf()));
        basis_values_["PHI_XZ"] = boost::shared_ptr<Matrix> (new Matrix("PHI_XZ", max_points_, primary_->nbf()));
        basis_values_["PHI_YY"] = boost::shared_ptr<Matrix> (new Matrix("PHI_YY", max_points_, primary_->nbf()));
        basis_values_["PHI_YZ"] = boost::shared_ptr<Matrix> (new Matrix("PHI_YZ", max_points_, primary_->nbf()));
        basis_values_["PHI_ZZ"] = boost::shared_ptr<Matrix> (new Matrix("PHI_ZZ", max_points_, primary_->nbf()));
        basis_temps_["PHI_XX"] = boost::shared_ptr<Matrix> (new Matrix("PHI_XX", max_points_, max_cart));
        basis_temps_["PHI_XY"] = boost::shared_ptr<Matrix> (new Matrix("PHI_XY", max_points_, max_cart));
        basis_temps_["PHI_XZ"] = boost::shared_ptr<Matrix> (new Matrix("PHI_XZ", max_points_, max_cart));
        basis_temps_["PHI_YY"] = boost::shared_ptr<Matrix> (new Matrix("PHI_YY", max_points_, max_cart));
        basis_temps_["PHI_YZ"] = boost::shared_ptr<Matrix> (new Matrix("PHI_YZ", max_points_, max_cart));
        basis_temps_["PHI_ZZ"] = boost::shared_ptr<Matrix> (new Matrix("PHI_ZZ", max_points_, max_cart));
    }

    if (deriv_ >= 3) 
        throw PSIEXCEPTION("PointFunctions: Only up to Hessians are currently supported"); 
}
boost::shared_ptr<Matrix> PointFunctions::basis_value(const std::string& key)
{
    return basis_values_[key];
}
void PointFunctions::computePoints(boost::shared_ptr<BlockOPoints> block)
{
    int max_am = primary_->max_am();
    int max_cart = (max_am + 1) * (max_am + 2) / 2;

    int nso = primary_->nbf();
 
    int npoints = block->npoints();
    npoints_ = npoints;
    double *restrict x = block->x();
    double *restrict y = block->y();
    double *restrict z = block->z();

    int maxL = primary_->max_am();

    double *restrict xc_pow = new double[maxL + 3];
    double *restrict yc_pow = new double[maxL + 3];
    double *restrict zc_pow = new double[maxL + 3];

    const std::vector<int>& shells = block->shells_local_to_global();

    int nsig_functions = block->functions_local_to_global().size();

    if (deriv_ == 0) {
        double** cartp = basis_temps_["PHI"]->pointer();
        double** purep = basis_values_["PHI"]->pointer();

        for (int P = 0; P < npoints; P++) {
            ::memset(static_cast<void*>(purep[P]),'\0',nsig_functions*sizeof(double));
        }

        int function_offset = 0;
        for (int Qlocal = 0; Qlocal < shells.size(); Qlocal++) {
            int Qglobal = shells[Qlocal];
            boost::shared_ptr<GaussianShell> Qshell = primary_->shell(Qglobal);
            Vector3 v     = Qshell->center();
            int L         = Qshell->am();
            int nQ        = Qshell->nfunction();
            int nprim     = Qshell->nprimitive();
            double *restrict alpha = Qshell->exps();
            double *restrict norm  = Qshell->coefs();
      
            const std::vector<boost::tuple<int,int,double> >& transform = spherical_transforms_[L];
 
            xc_pow[0] = 1.0;
            yc_pow[0] = 1.0;
            zc_pow[0] = 1.0;

            // Computation of points
            for (int P = 0; P < npoints; P++) {

                double xc = x[P] - v[0];
                double yc = y[P] - v[1];
                double zc = z[P] - v[2];                
                
                for (int LL = 1; LL < L + 1; LL++) {
                    xc_pow[LL] = xc_pow[LL - 1] * xc;
                    yc_pow[LL] = yc_pow[LL - 1] * yc;
                    zc_pow[LL] = zc_pow[LL - 1] * zc;
                } 

                double R2 = xc * xc + yc * yc + zc * zc;
                double S0 = 0.0;
                for (int K = 0; K < nprim; K++) {
                    S0 += norm[K] * exp(-alpha[K] * R2);
                }

                for (int i=0, index = 0; i<=L; ++i) {
                    int l = L-i;
                    for (int j=0; j<=i; ++j, ++index) {
                        int m = i-j;
                        int n = j;
                        
                        cartp[P][index] = S0 * xc_pow[l] * yc_pow[m] * zc_pow[n];
                    }
                }
            }    

            // Spherical transform
            for (int index = 0; index < transform.size(); index++) {
                int pureindex = boost::get<0>(transform[index]);
                int cartindex = boost::get<1>(transform[index]);
                double coef   = boost::get<2>(transform[index]);
                
                C_DAXPY(npoints,coef,&cartp[0][cartindex],max_cart,&purep[0][pureindex + function_offset],nso);
            }

            function_offset += nQ;
        }
    } else if (deriv_ == 1) {
        double** cartp = basis_temps_["PHI"]->pointer();
        double** cartxp = basis_temps_["PHI_X"]->pointer();
        double** cartyp = basis_temps_["PHI_Y"]->pointer();
        double** cartzp = basis_temps_["PHI_Z"]->pointer();
        double** purep = basis_values_["PHI"]->pointer();
        double** purexp = basis_values_["PHI_X"]->pointer();
        double** pureyp = basis_values_["PHI_Y"]->pointer();
        double** purezp = basis_values_["PHI_Z"]->pointer();
        
        for (int P = 0; P < npoints; P++) {
            ::memset(static_cast<void*>(purep[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(purexp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(pureyp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(purezp[P]),'\0',nsig_functions*sizeof(double));
        }

        int function_offset = 0;
        for (int Qlocal = 0; Qlocal < shells.size(); Qlocal++) {
            int Qglobal = shells[Qlocal];
            boost::shared_ptr<GaussianShell> Qshell = primary_->shell(Qglobal);
            Vector3 v     = Qshell->center();
            int L         = Qshell->am();
            int nQ        = Qshell->nfunction();
            int nprim     = Qshell->nprimitive();
            double *restrict alpha = Qshell->exps();
            double *restrict norm  = Qshell->coefs();
       
            const std::vector<boost::tuple<int,int,double> >& transform = spherical_transforms_[L];
 
            xc_pow[0] = 0.0;
            yc_pow[0] = 0.0;
            zc_pow[0] = 0.0;
            xc_pow[1] = 1.0;
            yc_pow[1] = 1.0;
            zc_pow[1] = 1.0;

            for (int P = 0; P < npoints; P++) {

                double xc = x[P] - v[0];
                double yc = y[P] - v[1];
                double zc = z[P] - v[2];                
                
                for (int LL = 2; LL < L + 2; LL++) {
                    xc_pow[LL] = xc_pow[LL - 1] * xc;
                    yc_pow[LL] = yc_pow[LL - 1] * yc;
                    zc_pow[LL] = zc_pow[LL - 1] * zc;
                } 

                double R2 = xc * xc + yc * yc + zc * zc;
                double V1 = 0.0;
                double V2 = 0.0;
                double V3 = 0.0;
                double T1,T2;
                for (int K = 0; K < nprim; K++) {
                    T1 =  norm[K] * exp(-alpha[K] * R2);
                    T2 =  -2.0 * alpha[K] * T1;
                    V1 += T1;
                    V2 += T2;
                }
                double S0 = V1;
                double SX = V2 * xc;
                double SY = V2 * yc;
                double SZ = V2 * zc;

                for (int i=0, index = 0; i<=L; ++i) {
                    int l = L-i+1;
                    for (int j=0; j<=i; ++j, ++index) {
                        int m = i-j+1;
                        int n = j+1;
                       
                        int lp = l - 1;
                        int mp = m - 1;
                        int np = n - 1;
                        
                        double xyz = xc_pow[l] * yc_pow[m] * zc_pow[n]; 
                        cartp[P][index] = S0 * xyz; 
                        cartxp[P][index] = S0 * lp * xc_pow[l-1] * yc_pow[m] * zc_pow[n] + SX * xyz; 
                        cartyp[P][index] = S0 * mp * xc_pow[l] * yc_pow[m-1] * zc_pow[n] + SY * xyz; 
                        cartzp[P][index] = S0 * np * xc_pow[l] * yc_pow[m] * zc_pow[n-1] + SZ * xyz; 
                    }
                }
            }    

            // Spherical transform
            for (int index = 0; index < transform.size(); index++) {
                int pureindex = boost::get<0>(transform[index]);
                int cartindex = boost::get<1>(transform[index]);
                double coef   = boost::get<2>(transform[index]);
                
                C_DAXPY(npoints,coef,&cartp[0][cartindex],max_cart,&purep[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartxp[0][cartindex],max_cart,&purexp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartyp[0][cartindex],max_cart,&pureyp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartzp[0][cartindex],max_cart,&purezp[0][pureindex + function_offset],nso);
            }

            function_offset += nQ;
        }
    } else if (deriv_ == 2) {
        double** cartp = basis_temps_["PHI"]->pointer();
        double** cartxp = basis_temps_["PHI_X"]->pointer();
        double** cartyp = basis_temps_["PHI_Y"]->pointer();
        double** cartzp = basis_temps_["PHI_Z"]->pointer();
        double** cartxxp = basis_temps_["PHI_XX"]->pointer();
        double** cartxyp = basis_temps_["PHI_XY"]->pointer();
        double** cartxzp = basis_temps_["PHI_XZ"]->pointer();
        double** cartyyp = basis_temps_["PHI_YY"]->pointer();
        double** cartyzp = basis_temps_["PHI_YZ"]->pointer();
        double** cartzzp = basis_temps_["PHI_ZZ"]->pointer();
        double** purep = basis_values_["PHI"]->pointer();
        double** purexp = basis_values_["PHI_X"]->pointer();
        double** pureyp = basis_values_["PHI_Y"]->pointer();
        double** purezp = basis_values_["PHI_Z"]->pointer();
        double** purexxp = basis_values_["PHI_XX"]->pointer();
        double** purexyp = basis_values_["PHI_XY"]->pointer();
        double** purexzp = basis_values_["PHI_XZ"]->pointer();
        double** pureyyp = basis_values_["PHI_YY"]->pointer();
        double** pureyzp = basis_values_["PHI_YZ"]->pointer();
        double** purezzp = basis_values_["PHI_ZZ"]->pointer();
        
        for (int P = 0; P < npoints; P++) {
            ::memset(static_cast<void*>(purep[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(purexp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(pureyp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(purezp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(purexxp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(purexyp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(purexzp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(pureyyp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(pureyzp[P]),'\0',nsig_functions*sizeof(double));
            ::memset(static_cast<void*>(purezzp[P]),'\0',nsig_functions*sizeof(double));
        }

        int function_offset = 0;
        for (int Qlocal = 0; Qlocal < shells.size(); Qlocal++) {
            int Qglobal = shells[Qlocal];
            boost::shared_ptr<GaussianShell> Qshell = primary_->shell(Qglobal);
            Vector3 v     = Qshell->center();
            int L         = Qshell->am();
            int nQ        = Qshell->nfunction();
            int nprim     = Qshell->nprimitive();
            double *restrict alpha = Qshell->exps();
            double *restrict norm  = Qshell->coefs();
       
            const std::vector<boost::tuple<int,int,double> >& transform = spherical_transforms_[L];
 
            xc_pow[0] = 0.0;
            yc_pow[0] = 0.0;
            zc_pow[0] = 0.0;
            xc_pow[1] = 0.0;
            yc_pow[1] = 0.0;
            zc_pow[1] = 0.0;
            xc_pow[2] = 1.0;
            yc_pow[2] = 1.0;
            zc_pow[2] = 1.0;

            for (int P = 0; P < npoints; P++) {

                double xc = x[P] - v[0];
                double yc = y[P] - v[1];
                double zc = z[P] - v[2];                
                
                for (int LL = 3; LL < L + 3; LL++) {
                    xc_pow[LL] = xc_pow[LL - 1] * xc;
                    yc_pow[LL] = yc_pow[LL - 1] * yc;
                    zc_pow[LL] = zc_pow[LL - 1] * zc;
                } 

                double R2 = xc * xc + yc * yc + zc * zc;
                double V1 = 0.0;
                double V2 = 0.0;
                double V3 = 0.0;
                double T1,T2,T3;
                for (int K = 0; K < nprim; K++) {
                    T1 =  norm[K] * exp(-alpha[K] * R2);
                    T2 =  -2.0 * alpha[K] * T1;
                    T3 =  -2.0 * alpha[K] * T2;
                    V1 += T1;
                    V2 += T2;
                    V3 += T3;
                }
                double S0 = V1;
                double SX = V2 * xc;
                double SY = V2 * yc;
                double SZ = V2 * zc;
                double SXX = V3 * xc * xc;
                double SXY = V3 * xc * yc;
                double SXZ = V3 * xc * zc;
                double SYY = V3 * yc * yc;
                double SYZ = V3 * yc * zc;
                double SZZ = V3 * zc * zc;
                double SA = V3;

                for (int i=0, index = 0; i<=L; ++i) {
                    int l = L-i+2;
                    for (int j=0; j<=i; ++j, ++index) {
                        int m = i-j+2;
                        int n = j+2;
                      
                        int lp = l - 2;
                        int mp = m - 2;
                        int np = n - 2;
 
                        double xyz = xc_pow[l] * yc_pow[m] * zc_pow[n]; 
                        cartp[P][index] = S0 * xyz; 
                        cartxp[P][index] = S0 * lp * xc_pow[l-1] * yc_pow[m] * zc_pow[n] + SX * xyz; 
                        cartyp[P][index] = S0 * mp * xc_pow[l] * yc_pow[m-1] * zc_pow[n] + SY * xyz; 
                        cartzp[P][index] = S0 * np * xc_pow[l] * yc_pow[m] * zc_pow[n-1] + SZ * xyz; 
                        cartxyp[P][index] = S0 * lp * mp * xc_pow[l-1] * yc_pow[m-1] * zc_pow[n] + SY * lp * xc_pow[l-1] * yc_pow[m] * zc_pow[n] +
                                            SX * mp * xc_pow[l] * yc_pow[m-1] * zc_pow[n] + SXY * xyz; 
                        cartxzp[P][index] = S0 * lp * np * xc_pow[l-1] * yc_pow[m] * zc_pow[n-1] + SZ * lp * xc_pow[l-1] * yc_pow[m] * zc_pow[n] +
                                            SX * np * xc_pow[l] * yc_pow[m] * zc_pow[n-1] + SXZ * xyz; 
                        cartyzp[P][index] = S0 * mp * np * xc_pow[l] * yc_pow[m-1] * zc_pow[n-1] + SZ * mp * xc_pow[l] * yc_pow[m-1] * zc_pow[n] +
                                            SX * np * xc_pow[l] * yc_pow[m] * zc_pow[n-1] + SYZ * xyz; 
                        cartxxp[P][index] = S0 * lp * (lp - 1) * xc_pow[l-2] * yc_pow[m] * zc_pow[n] + SA * lp * xyz +
                                            SA * (lp + 1) * xyz + SXX * xyz; 
                        cartyyp[P][index] = S0 * mp * (mp - 1) * xc_pow[l] * yc_pow[m-2] * zc_pow[n] + SA * mp * xyz +
                                            SA * (mp + 1) * xyz + SYY * xyz; 
                        cartzzp[P][index] = S0 * np * (np - 1) * xc_pow[l] * yc_pow[m] * zc_pow[n-2] + SA * np * xyz +
                                            SA * (np + 1) * xyz + SZZ * xyz; 
                    }
                }
            }    

            // Spherical transform
            for (int index = 0; index < transform.size(); index++) {
                int pureindex = boost::get<0>(transform[index]);
                int cartindex = boost::get<1>(transform[index]);
                double coef   = boost::get<2>(transform[index]);
                
                C_DAXPY(npoints,coef,&cartp[0][cartindex],max_cart,&purep[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartxp[0][cartindex],max_cart,&purexp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartyp[0][cartindex],max_cart,&pureyp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartzp[0][cartindex],max_cart,&purezp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartxxp[0][cartindex],max_cart,&purexxp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartxyp[0][cartindex],max_cart,&purexyp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartxzp[0][cartindex],max_cart,&purexzp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartyyp[0][cartindex],max_cart,&pureyyp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartyzp[0][cartindex],max_cart,&pureyzp[0][pureindex + function_offset],nso);
                C_DAXPY(npoints,coef,&cartzzp[0][cartindex],max_cart,&purezzp[0][pureindex + function_offset],nso);
            }

            function_offset += nQ;
        }
    }

    delete[] xc_pow;
    delete[] yc_pow;
    delete[] zc_pow;
}
void PointFunctions::print(FILE* out, int print) const
{
    fprintf(out, "   => PointFunctions: Derivative = %d, Max Points = %d <=\n\n", deriv_, max_points_);

    fprintf(out, "    Basis Values:\n");
    for (std::map<std::string, boost::shared_ptr<Matrix> >::const_iterator it = basis_values_.begin();
        it != basis_values_.end(); it++) {
        fprintf(out, "    %s\n", (*it).first.c_str());
        if (print > 3) {
            (*it).second->print();
        }
    }
    fprintf(out,"\n\n");
}

} // Namespace psi
