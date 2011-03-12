/*
 *  hf.h
 *  matrix
 *
 *  Created by Justin Turney on 4/9/08.
 *  Copyright 2008 by Justin M. Turney, Ph.D.. All rights reserved.
 *
 */
//#define _MKL

#ifndef HF_H
#define HF_H

#include <libpsio/psio.hpp>
#include <libmints/wavefunction.h>
#include <libmints/basisset.h>
#include <libdiis/diismanager.h>
#include <psi4-dec.h>

using namespace psi;

namespace psi {
    class TwoBodySOInt;
    namespace scf {

class PseudospectralHF;
class PKIntegrals;
class DFHF; 

class HF : public Wavefunction {
protected:
    SharedMatrix H_;
    SharedMatrix S_;
    SharedMatrix Shalf_;
    SharedMatrix X_;
    SharedMatrix Sphalf_;

    /// Previous iteration's energy and current energy
    double Eold_;
    double E_;

    /// The RMS error in the density
    double Drms_;

    /// Max number of iterations for HF
    int maxiter_;

    /// Current Iteration
    int iteration_;

    /// Nuclear repulsion energy
    double nuclearrep_;

    /// DOCC vector from input (if found)
    bool input_docc_;

    /// SOCC vector from input (if found)
    bool input_socc_;

    /// Number of alpha and beta electrons
    int nalpha_, nbeta_;
    /// Number of alpha and beta electrons per irrep
    //int nalphapi_[8], nbetapi_[8];

    //Initial SAD doubly occupied may be more than ndocc
    int sad_nocc_[8];

    //Canonical or Symmetric orthogonalization?
    bool canonical_X_;

    /// Mapping arrays
    int *so2symblk_;
    int *so2index_;

    /// SCF algorithm type
    std::string scf_type_;

    /// Perturb the Hamiltonian?
    int perturb_h_;
    /// How big of a perturbation
    double lambda_;
    /// With what...
    enum perturb { nothing, dipole_x, dipole_y, dipole_z };
    perturb perturb_;

    /// The value below which integrals are neglected
    double integral_threshold_;


    /// The SO integral generator.  Only ever constructed if needed
    boost::shared_ptr<TwoBodySOInt> eri_;

    /// Pseudospectral stuff
    shared_ptr<PseudospectralHF> pseudospectral_;
    /// DF stuff
    shared_ptr<DFHF> df_;
    /// PK Matrix approach
    shared_ptr<PKIntegrals> pk_integrals_;

    /// DIIS manager intiialized?
    bool initialized_diis_manager_;
    /// DIIS manager for all SCF wavefunctions
    boost::shared_ptr<DIISManager> diis_manager_;

    /// How many min vectors for DIIS
    int min_diis_vectors_;
    /// How many max vectors for DIIS
    int max_diis_vectors_;
    /// When do we start collecting vectors for DIIS
    int diis_start_;
    /// Are we even using DIIS?
    int diis_enabled_;

public:
    /// Nuclear contributions
    SimpleVector nuclear_dipole_contribution_;
    SimpleVector nuclear_quadrupole_contribution_;

    /// Formation of H is the same regardless of RHF, ROHF, UHF
    void form_H();

    /// Formation of S^+1/2 and S^-1/2 are the same
    void form_Shalf();

    /// Perform casting of basis set if desired.
    shared_ptr<Matrix> dualBasisProjection(SharedMatrix _C, int *_noccpi, shared_ptr<BasisSet> _old, shared_ptr<BasisSet> _new);

    /// UHF Atomic Density Matrix for SAD
    /// returns atomic_basis->nbf() x atomic_basis_->nbf() double array of approximate atomic density (summed over spin)
    void getUHFAtomicDensity(shared_ptr<BasisSet> atomic_basis, int n_electrons, int multiplicity, double** D);
    // Computes the C and D matrix in place for SAD Atomic UHF
    void atomicUHFHelperFormCandD(int nelec, int norbs,double** Shalf, double**F, double** C, double** D);

    /// Set the amount of information to print
    void set_print(const int n) {print_ = n;}

    /// The number of iterations needed to reach convergence
    int iterations_needed() {return iterations_needed_;}

    /// The RMS error in the density
    double rms_density_error() {return Drms_;}
protected:
    /// Common initializer
    void common_init();

    /// Clears memory and closes files (Should they be open) prior to correlated code execution
    virtual void finalize();

    /// Figure out how to occupy the orbitals in the absence of DOCC and SOCC
    void find_occupation();

    /// Determine how many core and virtual orbitals to freeze
    void compute_fcpi();
    void compute_fvpi();

    /// Print the orbitals energies and symmetries
    void print_orbitals(const char *header, int *&irrep_count,
                        const std::vector<std::pair<double, int> >& evals,
                        int start, int end);

    /// Prints some opening information
    void print_header();

    /// The amout of information to print
    int print_;

    /// The number of electrons
    int nelectron_;

    /// The charge of the system
    int charge_;

    /// The multiplicity of the system (specified as 2 Ms + 1)
    int multiplicity_;

    /// The number of iterations need to reach convergence
    int iterations_needed_;

    /// Compute energy for the iteration.
    virtual double compute_E() = 0;

    /// Save the current density and energy.
    virtual void save_density_and_energy() = 0;

    /// SAD Guess and propagation
    void compute_SAD_guess();

    /** Read in C from checkpoint. Default implementation works for RHF and ROHF. UHF needs to read in additional C.
     *  If unable to load C from checkpoint, will call form_C to compute the value.
     *  If unable to load call compute_initial_E(), else loads SCF energy from checkpoint.
     * @returns true if successfully loaded from checkpoint, else false.
     */
    virtual bool load_or_compute_initial_C();

    /** Computes the density matrix (D_) */
    virtual void form_D() =0;

    /** Compute the MO coefficients (C_) */
    virtual void form_C() =0;

    /** Computes the Fock matrix */
    virtual void form_F() =0;

    /** Computes the initial MO coefficients (default is to call form_C) */
    virtual void form_initial_C() { form_C(); }

    /** Forms the G matrix */
    virtual void form_G() =0;

    /** Computes the initial energy. */
    virtual double compute_initial_E() { return 0.0; }

    /** Test convergence of the wavefunction */
    virtual bool test_convergency() { return false; }

    /** Saves information to the checkpoint file */
    virtual void save_information() {}

    /** Handles forming the PK matrix */
    virtual void form_PK() {}

    /** Save the Fock matrix to the DIIS object */
    virtual void save_fock() {}

    /** Performs DIIS extrapolation */
    virtual bool diis() { return false; }

    virtual void save_dual_basis_projection() {}
    virtual void save_sapt_info() {}

    /** Saves all wavefunction information to the checkpoint file*/
    void dump_to_checkpoint();

    /** Computes the J and/or K matrices according to the scf_type keyword and the functor passed in*/
    template <class JKFunctor> void process_tei(JKFunctor & functor);

    inline int integral_type(int i, int j, int k, int l)
    {
        int type;

        if (i == j && i == k && i == l)     // (ij|kl)  (11|11)
            type = 1;
        else if (i == j && k == l && i > k) // (ij|kl)  (22|11)
            type = 2;
        else if (i == j && i == k && i > l) // (ij|kl)  (22|21)
            type = 3;
        else if (j == k && j == l && i > j) // (ij|kl)  (21|11)
            type = 4;
        else if (i == k && j == l)          // (ij|kl)  (21|21)
            type = 5;
        else if (i == j)                    // (ij|kl)  (33|21)
            type = 6;
        else if (j >  k && k == l)          // (ij|kl)  (32|11)
            type = 7;
        else if (k == l)                    // (ij|kl)  (31|22)
            type = 8;
        else if (i == k)                    // (ij|kl)  (32|31)
            type = 9;
        else if (j == k)                    // (ij|kl)  (32|21)
            type = 10;
        else if (j == l)                    // (ij|kl)  (31|21)
            type = 11;
        else if (j >  k)                    // (ij|kl)  (43|21)
            type = 12;
        else if (j >  l)                    // (ij|kl)  (42|31)
            type = 13;
        else                                // (ij|kl)  (41|32)
            type = 14;

        return type;
    }
public:
    HF(Options& options, shared_ptr<PSIO> psio, shared_ptr<Chkpt> chkpt);
    HF(Options& options, shared_ptr<PSIO> psio);

    virtual ~HF();

    virtual double compute_energy();
};

}} // Namespaces

#endif
