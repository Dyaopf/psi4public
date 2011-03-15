#include "pointgrp.h"
#include "petitelist.h"
#include "sobasis.h"
#include "molecule.h"
#include "basisset.h"
#include "gshell.h"
#include "dimension.h"

#include <psi4-dec.h>
#include <cstdio>

using namespace boost;
using namespace psi;

SOTransform::SOTransform()
{
    naoshell_allocated = 0;
    naoshell = 0;
    aoshell = 0;
}

SOTransform::~SOTransform()
{
    delete[] aoshell;
}

void SOTransform::set_naoshell(int n)
{
    naoshell = 0;
    delete[] aoshell;
    naoshell_allocated = n;
    aoshell = new SOTransformShell[n];
}

void SOTransform::add_transform(int aoshellnum, int irrep,
                                double coef, int aofunc, int sofunc)
{
//    fprintf(outfile, "SOTransform::add_transform(aoshellnum = %d, irrep = %d, coef = %lf, aofunc = %d, sofunc = %d)\n", aoshellnum, irrep, coef, aofunc, sofunc);

    int i;
    for (i=0; i<naoshell; i++) {
        if (aoshell[i].aoshell == aoshellnum) break;
    }
    if (i>=naoshell_allocated) {
        throw PSIEXCEPTION("SOTransform::add_transform: allocation too small");
    }
    aoshell[i].add_func(irrep,coef,aofunc,sofunc);
    aoshell[i].aoshell = aoshellnum;
    if (i==naoshell) naoshell++;
}

///////////////////////////////////////////////////////////////////////////////

SOTransformShell::SOTransformShell()
{
    nfunc = 0;
    func = 0;
}

SOTransformShell::~SOTransformShell()
{
    delete[] func;
}

void SOTransformShell::add_func(int irrep, double coef, int aofunc, int sofunc)
{
    SOTransformFunction *newfunc = new SOTransformFunction[nfunc+1];
    for (int i=0; i<nfunc; i++) newfunc[i] = func[i];
    delete[] func;
    func = newfunc;
    func[nfunc].irrep = irrep;
    func[nfunc].coef = coef;
    func[nfunc].aofunc = aofunc;
    func[nfunc].sofunc = sofunc;
    nfunc++;
}

///////////////////////////////////////////////////////////////////////////////

SOBasisSet::SOBasisSet(const boost::shared_ptr<BasisSet> &basis, const IntegralFactory *integral)
    : basis_(basis), integral_(integral)
{
    init();
}

SOBasisSet::SOBasisSet(const boost::shared_ptr<BasisSet> &basis, const boost::shared_ptr<IntegralFactory> &integral)
    : basis_(basis), integral_(integral.get())
{
    init();
}

void SOBasisSet::init()
{
    int i,j,k;

    boost::shared_ptr<Molecule> mol = basis_->molecule();

    CharacterTable ct = mol->point_group()->char_table();
    nirrep_ = ct.nirrep();

    // count the number of so shells
    nshell_ = 0;
    for (i=0; i<mol->nunique(); i++) {
        nshell_ += basis_->nshell_on_center(mol->unique(i));
    }

    //=----- Begin debug printing -----=
//    fprintf(outfile, "SOBasis:\n");
//    fprintf(outfile, "nshell_ = %d\n", nshell_);
    //=-----  End debug printing  -----=

    // map each ao shell to an so shell
    int *aoshell_to_soshell = new int[basis_->nshell()];
    int soshell = 0;
    for (i=0; i<mol->nunique(); i++) {
        for (j=0; j<basis_->nshell_on_center(mol->unique(i)); j++) {
            for (k=0; k<mol->nequivalent(i); k++) {
                int aoshell = basis_->shell_on_center(mol->equivalent(i,k),j);
                aoshell_to_soshell[aoshell] = soshell;
//                fprintf(outfile, "i = %d j = %d k = %d aoshell = %d soshell = %d, mol->equivalent = %d\n",
//                        i, j, k, aoshell, soshell, mol->equivalent(i,k));
            }
            soshell++;
        }
    }

    //=----- Begin debug printing -----=
//    fprintf(outfile, "Final aoshell_to_soshell:\n");
//    for (i = 0; i < basis_->nshell(); ++i) {
//        fprintf(outfile, "aoshell_to_soshell[%d] = %d\n", i, aoshell_to_soshell[i]);
//    }
    //=-----  End debug printing  -----=

    ncomp_ = new int[nirrep_];
    for (i=0; i<nirrep_; i++) {
        ncomp_[i] = ct.gamma(i).degeneracy();
        if (ncomp_[i] != 1) {
            throw PSIEXCEPTION("SOBasis::SOBasis: not tested for degenerate point groups");
        }
    }

    naofunc_ = new int[nshell_];
    memset(naofunc_, 0, sizeof(int)*nshell_);

    nfunc_ = new int*[nshell_];
    funcoff_ = new int*[nshell_];
    for (i=0; i<nshell_; i++) {
        nfunc_[i] = new int[nirrep_];
        funcoff_[i] = new int[nirrep_];
        for (j=0; j<nirrep_; j++) {
            nfunc_[i][j] = 0;
        }
    }

    petite_ = shared_ptr<PetiteList>(new PetiteList(basis_, integral_));

//    petite->print();

    int nblocks = petite_->nblocks();
    SO_block *soblocks = petite_->aotoso_info();
//    for (i=0; i<nblocks; ++i) {
//        fprintf(outfile, "soblock[%d]\n", i); fflush(outfile);
//        soblocks[i].print("");
//    }

    trans_ = new SOTransform[nshell_];
    for (i=0; i<nblocks; i++) {
        for (j=0; j<soblocks[i].len; j++) {
            if (soblocks[i].so[j].length == 0) continue;
            int bfn0 = soblocks[i].so[j].cont[0].bfn;
            int aoshell0 = basis_->function_to_shell(bfn0);
            int soshell0 = aoshell_to_soshell[aoshell0];
            int atom0 = basis_->shell_to_center(aoshell0);
            int nequiv0 = mol->nequivalent(mol->atom_to_unique(atom0));
            trans_[soshell0].set_naoshell(nequiv0);

//            fprintf(outfile, "i = %d j = %d bfn0 = %d aoshell0 = %d soshell0 = %d atom0 = %d nequiv0 = %d\n", i, j, bfn0, aoshell0, soshell0, atom0, nequiv0);
        }
    }

    int nfuncall = 0;
    for (i=0; i<nblocks; i++) {
        int irrep = ct.which_irrep(i);
//        fprintf(outfile, "i = %d, irrep = %d...I think they should be equal.\n", i, irrep); fflush(outfile);
        for (j=0; j<soblocks[i].len; j++) {
            if (soblocks[i].so[j].length == 0) continue;
            int bfn0 = soblocks[i].so[j].cont[0].bfn;
            int aoshell0 = basis_->function_to_shell(bfn0);
            int soshell0 = aoshell_to_soshell[aoshell0];
            int sofunc = nfunc_[soshell0][irrep];

            int naofunc = basis_->shell(aoshell0)->nfunction();
            if (naofunc_[soshell0] && (naofunc_[soshell0] != naofunc)) {
                throw PSIEXCEPTION("SOBasis::SOBasis: mismatch in naofunc");
            }
            naofunc_[soshell0] = naofunc;

            nfunc_[soshell0][irrep]++;
            nfuncall++;

            for (k=0; k<soblocks[i].so[j].length; k++) {
                int bfn = soblocks[i].so[j].cont[k].bfn;
                double coef = soblocks[i].so[j].cont[k].coef;
                int aoshell = basis_->function_to_shell(bfn);
                // This might need to be shell_to_basis_function.
                int aoshellfunc = bfn - basis_->shell_to_basis_function(aoshell);
                int soshell = aoshell_to_soshell[aoshell];

                if (soshell != soshell0) {
                    throw PSIEXCEPTION("SOBasis::SOBasis: shell changed");
                }

//                fprintf(outfile, "add_transform(...): aoshell = %d irrep = %d coef = %lf aoshellfunc = %d sofunc = %d\n",
//                       aoshell,irrep, coef,aoshellfunc,sofunc);
                trans_[soshell].add_transform(aoshell, irrep, coef,aoshellfunc,sofunc);
            }
        }
    }

    if (nfuncall != basis_->nbf()) {
        throw PSIEXCEPTION("SOBasis::SOBasis: miscounted number of functions");
    }

    delete[] soblocks;
    delete[] aoshell_to_soshell;

    for (i=0; i<nshell_; i++) {
        funcoff_[i][0] = 0;
        for (j=1; j<nirrep_; j++) {
            funcoff_[i][j] = funcoff_[i][j-1] + nfunc_[i][j-1];
//            fprintf(outfile, "funcoff_[%d][%d] = %d\n", i, j, funcoff_[i][j]);
        }
    }

    func_ = new int[nshell_];
    irrep_ = new int[basis_->nbf()];
    func_within_irrep_ = new int[basis_->nbf()];
    nfunc_in_irrep_ = new int[nirrep_];

    for (i=0; i<nirrep_; i++) nfunc_in_irrep_[i] = 0;

    if (nshell_) {
        func_[0] = 0;
        for (i=1; i<nshell_; i++) {
            func_[i] = func_[i-1] + nfunction(i-1);
//            fprintf(outfile, "func_[%d] = %d\n", i, func_[i]);
        }
        int ibasis_ = 0;
        for (i=0; i<nshell_; i++) {
            for (j=0; j<nirrep_; j++) {
                for (k=0; k<nfunc_[i][j]; k++,ibasis_++) {
                    irrep_[ibasis_] = j;
                    func_within_irrep_[ibasis_] = nfunc_in_irrep_[j]++;
//                    fprintf(outfile, "irrep_[%d] = %d func_within_irrep_[%d] = %d\n", ibasis_, j, ibasis_, func_within_irrep_[ibasis_]);
                }
            }
        }
    }
}

SOBasisSet::~SOBasisSet()
{
    for (int i=0; i<nshell_; i++) {
        delete[] nfunc_[i];
        delete[] funcoff_[i];
    }
    delete[] nfunc_;
    delete[] funcoff_;
    delete[] naofunc_;
    delete[] ncomp_;
    delete[] trans_;
    delete[] func_;
    delete[] irrep_;
    delete[] func_within_irrep_;
    delete[] nfunc_in_irrep_;
}

int SOBasisSet::max_nfunction_in_shell() const
{
    int maxn = 0;
    for (int i=0; i<nshell_; i++) {
        int n = nfunction(i);
        if (n > maxn) maxn = n;
    }
    return maxn;
}

int SOBasisSet::nfunction(int ishell) const
{
    int n=0;
    for (int i=0; i<nirrep_; i++) {
        n += nfunc_[ishell][i];
    }
    return n;
}

void SOBasisSet::print(FILE *out) const
{
    int i,j,k;

    fprintf(out, "  SOBasis:\n");
    fprintf(out, "    nshell(SO) = %d\n", nshell_);
    fprintf(out, "    nirrep = %d\n", nirrep_);

    fprintf(out, "    ncomp = [");
    for (i=0; i<nirrep_; i++)
        fprintf(out, " %3d", ncomp_[i]);
    fprintf(out, " ]\n");

    fprintf(out, "    nfunc:\n");
    for (i=0; i<nshell_; i++) {
        fprintf(out, "      %3d:", i);
        for (j=0; j<nirrep_; j++)
            fprintf(out, "  %3d", nfunc_[i][j]);
        fprintf(out, "\n");
    }

    fprintf(out, "    irrep             = [");
    for (i=0; i<basis_->nbf(); ++i) {
        fprintf(out, " %4d", irrep_[i]);
    }
    fprintf(out, "]\n");

    fprintf(out, "    func              = [");
    for (i=0; i<nshell_; ++i) {
        fprintf(out, " %4d", func_[i]);
    }
    fprintf(out, "]\n");

    fprintf(out, "    func_within_irrep = [");
    for (i=0; i<basis_->nbf(); ++i) {
        fprintf(out, " %4d", func_within_irrep_[i]);
    }
    fprintf(out, "]\n");

    fprintf(out, "    nfunc_in_irrep    = [");
    for (i=0; i<nirrep_; ++i) {
        fprintf(out, " %4d", nfunc_in_irrep_[i]);
    }
    fprintf(out, "]\n");

    fprintf(out, "    funcoff           = [\n");
    for (i=0; i<nshell_; i++) {
        fprintf(out, "      %3d:", i);
        for (j=0; j<nirrep_; j++)
            fprintf(out, "  %3d", funcoff_[i][j]);
        fprintf(out, "\n");
    }

    fprintf(out, "    transform:\n");
    for (i=0; i<nshell_; i++) {
        if (i>0) fprintf(out, "\n");
        for (j=0; j<trans_[i].naoshell; j++) {
            for (k=0; k<trans_[i].aoshell[j].nfunc; k++) {
                fprintf(out, "      SO(%3d %2d %d [%2d]) += % 12.8f * AO(%3d %2d)\n",
                        i,
                        trans_[i].aoshell[j].func[k].sofunc,
                        trans_[i].aoshell[j].func[k].irrep,
                        function_offset_within_shell(
                            i, trans_[i].aoshell[j].func[k].irrep)
                        + trans_[i].aoshell[j].func[k].sofunc,
                        trans_[i].aoshell[j].func[k].coef,
                        trans_[i].aoshell[j].aoshell,
                        trans_[i].aoshell[j].func[k].aofunc);
            }
        }
    }
}

Dimension SOBasisSet::dimension() const
{
    shared_ptr<PetiteList> petite = shared_ptr<PetiteList>(new PetiteList(basis_, integral_));
    return petite->SO_basisdim();
}

const boost::shared_ptr<PetiteList> SOBasisSet::petitelist() const
{
    return petite_;
}
