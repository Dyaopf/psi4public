#ifndef __rohf_psi_h__
#define __rohf_psi_h__

#include <vector>
#include <libpsio/psio.hpp>
#include "hf.h"

namespace boost {
template<class T> class shared_ptr;
}

namespace psi { namespace scf {

class ROHF : public HF {
protected:
    SharedMatrix Feff_;
    SharedMatrix Dt_old_;
    SharedMatrix Dt_;
    SharedMatrix Ga_;
    SharedMatrix Gb_;
    SharedMatrix Ka_;
    SharedMatrix Kb_;
    SharedMatrix Da_;
    SharedMatrix Db_;
    SharedMatrix moFa_;
    SharedMatrix moFb_;

    void form_initialF();
    void form_initial_C();
    void form_C();
    void form_D();
    double compute_initial_E();
    double compute_E();

    void form_G();
    void form_F();

    void save_fock();
    bool diis();

    bool test_convergency();

    void save_information();
    // Finalize memory/files
    virtual void finalize();

    void save_density_and_energy();

    void common_init();
public:
    ROHF(Options& options, boost::shared_ptr<PSIO> psio, boost::shared_ptr<Chkpt> chkpt);
    ROHF(Options& options, boost::shared_ptr<PSIO> psio);
    virtual ~ROHF();
    virtual bool restricted() const { return false; }
};

}}

#endif
