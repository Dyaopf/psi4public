#include <libplugin/plugin.h>
#include "psi4-dec.h"
#include <libparallel/parallel.h>
#include <liboptions/liboptions.h>
#include <libmints/mints.h>
#include <libpsio/psio.h>

INIT_PLUGIN

using namespace boost;

namespace psi{ namespace aointegrals{

extern "C" int
read_options(std::string name, Options &options)
{
    if (name == "AOINTEGRALS"|| options.read_globals()) {
        /*- The amount of information printed
            to the output file -*/
        options.add_int("PRINT", 1);
        /*- Whether to compute two-electron integrals -*/
        options.add_bool("DO_TEI", true);
    }

    return true;
}

extern "C" PsiReturnType
aointegrals(Options &options)
{
    int print = options.get_int("PRINT");
    int doTei = options.get_bool("DO_TEI");

    shared_ptr<Molecule> molecule = Process::environment.molecule();

    // Form basis object:
    shared_ptr<BasisSet> aoBasis = BasisSet::pyconstruct_orbital(molecule, "BASIS", options.get_str("BASIS"));

    // The integral factory oversees the creation of integral objects
    shared_ptr<IntegralFactory> integral(new IntegralFactory
            (aoBasis, aoBasis, aoBasis, aoBasis));

    // N.B. This should be called after the basis has been built, because the geometry has not been
    // fully initialized until this time.
    molecule->print();
    int nbf[] = { aoBasis->nbf() };
    double nucrep = molecule->nuclear_repulsion_energy();
    outfile->Printf("\n    Nuclear repulsion energy: %16.8f\n\n", nucrep);

    // The matrix factory can create matrices of the correct dimensions...
    shared_ptr<MatrixFactory> factory(new MatrixFactory);
    factory->init_with(1, nbf, nbf);

    // Form the one-electron integral objects from the integral factory
    shared_ptr<OneBodyAOInt> sOBI(integral->ao_overlap());
    shared_ptr<OneBodyAOInt> tOBI(integral->ao_kinetic());
    shared_ptr<OneBodyAOInt> vOBI(integral->ao_potential());
    // Form the one-electron integral matrices from the matrix factory
    shared_ptr<Matrix> sMat(factory->create_matrix("Overlap"));
    shared_ptr<Matrix> tMat(factory->create_matrix("Kinetic"));
    shared_ptr<Matrix> vMat(factory->create_matrix("Potential"));
    shared_ptr<Matrix> hMat(factory->create_matrix("One Electron Ints"));
    // Compute the one electron integrals, telling each object where to store the result
    sOBI->compute(sMat);
    tOBI->compute(tMat);
    vOBI->compute(vMat);

    sMat->print();
    tMat->print();
    vMat->print();

    // Form h = T + V by first cloning T and then adding V
    hMat->copy(tMat);
    hMat->add(vMat);
    hMat->print();

    if(doTei){
         outfile->Printf("\n  Two-electron Integrals\n\n");

        // Now, the two-electron integrals
        shared_ptr<TwoBodyAOInt> eri(integral->eri());
        // The buffer will hold the integrals for each shell, as they're computed
        const double *buffer = eri->buffer();
        // The iterator conveniently lets us iterate over functions within shells
        AOShellCombinationsIterator shellIter = integral->shells_iterator();
        int count=0;
        for (shellIter.first(); shellIter.is_done() == false; shellIter.next()) {
            // Compute quartet
            eri->compute_shell(shellIter);
            // From the quartet get all the integrals
            AOIntegralsIterator intIter = shellIter.integrals_iterator();
            for (intIter.first(); intIter.is_done() == false; intIter.next()) {
                int p = intIter.i();
                int q = intIter.j();
                int r = intIter.k();
                int s = intIter.l();
                outfile->Printf("\t(%2d %2d | %2d %2d) = %20.15f\n",
                    p, q, r, s, buffer[intIter.index()]);
                ++count;
            }
        }
        outfile->Printf("\n\tThere are %d unique integrals\n\n", count);
    }

    return Success;
}

}} // End Namespaces
