#include <boost/python.hpp>

#include <cstdlib>
#include <cstdio>
#include <string>

#include <psifiles.h>
#include <libciomr/libciomr.h>
#include <libpsio/psio.h>
#include <libchkpt/chkpt.h>
#include <libpsio/psio.hpp>
#include <libparallel/parallel.h>
#include <libchkpt/chkpt.hpp>
#include <libiwl/iwl.h>
#include <libqt/qt.h>

#include <libmints/mints.h>
#include <psi4-dec.h>

#include <libscf_solver/rhf.h>
#include <libscf_solver/rohf.h>
#include <libscf_solver/uhf.h>
#include <libscf_solver/cuhf.h>
#include <libscf_solver/ks.h>

using namespace boost;
using namespace std;

namespace psi { namespace scf {

PsiReturnType scf(Options & options, PyObject* pre, PyObject* post)
{
    tstart();

    boost::shared_ptr<PSIO> psio = PSIO::shared_object();

    string reference = options.get_str("REFERENCE");
    boost::shared_ptr<Wavefunction> scf;
    double energy;
    //bool parallel = options.get_bool("PARALLEL");

    if (reference == "RHF") {
        scf = boost::shared_ptr<Wavefunction>(new RHF(options, psio));
    }
    else if (reference == "ROHF") {
        scf = boost::shared_ptr<Wavefunction>(new ROHF(options, psio));
    }
    else if (reference == "UHF") {
        scf = boost::shared_ptr<Wavefunction>(new UHF(options, psio));
    }
    else if (reference == "CUHF") {
        scf = boost::shared_ptr<Wavefunction>(new CUHF(options, psio));
    }
    else if (reference == "RKS") {
        scf = boost::shared_ptr<Wavefunction>(new RKS(options, psio));
    }
    else if (reference == "UKS") {
        scf = boost::shared_ptr<Wavefunction>(new UKS(options, psio));
    }
    else {
        throw InputException("Unknown reference " + reference, "REFERENCE", __FILE__, __LINE__);
        energy = 0.0;
    }

    // Set this early because the callback mechanism uses it.
    Process::environment.set_reference_wavefunction(scf);

    if (pre)
        scf->add_preiteration_callback(pre);
    if (post)
        scf->add_postiteration_callback(post);

    energy = scf->compute_energy();

    // Set some environment variables
    Process::environment.globals["SCF ENERGY"] = energy;
    Process::environment.globals["CURRENT ENERGY"] = energy;

    // Shut down psi.

    tstop();

    return Success;
}

}}
