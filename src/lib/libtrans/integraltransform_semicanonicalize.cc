#include "integraltransform.h"
#include <libchkpt/chkpt.hpp>
#include <libciomr/libciomr.h>
#include <libqt/qt.h>
#include <libiwl/iwl.hpp>
#include "mospace.h"
#include "psifiles.h"
#include "spaceinfo.h"

using namespace psi;

/**
* This will read in the alpha and beta Fock matrices, which were computed during the
* two-electron SO integral sort and diagonalize the occ-occ and vir-vir blocks in the
* alpha and beta spaces.  This routine is called automatically if the TransformationType
* is set to Semicanonical.  The resulting integral transformation is then unrestricted.
*/
void
IntegralTransform::semicanonicalize()
{
    // C will hold the ROHF eigenvector, which is currently in _Ca
    double ***C = new double**[_nirreps];

    double *work  = init_array(3 * _nmo);
    double *evals = init_array(_nmo);
    double *temp  = init_array(_nTriMo);

    // The alpha MOs - the Fock matrix is on disk from the so TEI sort routine
    if(_print > 3) fprintf(outfile, "The alpha Fock matrix before semicanonicalization\n");
    IWL::read_one(_psio.get(), PSIF_OEI, PSIF_MO_A_FOCK, temp, _nTriMo, 0, _print > 3, outfile);
    for(int h = 0, moOffset = 0; h < _nirreps; ++h){
        if(_mopi[h] == 0) continue;
        C[h] = block_matrix(_sopi[h], _mopi[h]);
        memcpy(C[h][0], _Ca[h][0], _sopi[h] * _mopi[h] * sizeof(double));
        // The Alpha occ-occ block
        int nOccOrbs = _clsdpi[h] + _openpi[h];
        if(nOccOrbs){
            double **F = block_matrix(nOccOrbs, nOccOrbs);
            for(int i = 0; i < nOccOrbs; ++i){
                for(int j = 0; j <= i; ++j){
                    int ij = INDEX((i + moOffset), (j + moOffset));
                    F[i][j] = F[j][i] = temp[ij];
                }
            }
            double *evalPtr = &(evals[moOffset]);
            // Diagonalize the Fock matrix; on exit F is the rotation matrix to diagonalize F
            if(int stat = C_DSYEV('v','u', nOccOrbs, F[0], nOccOrbs, evalPtr, work, 3*_nmo)){
                fprintf(outfile,
                        "rotate(): Error in alpha Foo[%1d] diagonalization. stat = %d\n", h, stat);
                throw PsiException("Semicanonicalization error", __FILE__, __LINE__);
            }
            // Now rotate the orbitals using the eigenvectors of F
            C_DGEMM('n', 't', _sopi[h], nOccOrbs, nOccOrbs, 1.0, C[h][0],
                    _mopi[h], F[0], nOccOrbs, 0.0, _Ca[h][0], _mopi[h]);
            // Update the Fock Matrix
            for(int i = 0; i < nOccOrbs; ++i){
                for(int j = 0; j <= i; ++j){
                    int ij = INDEX((i + moOffset), (j + moOffset));
                    temp[ij] = i == j ? evalPtr[i] : 0.0;
                }
            }
            free_block(F);
        }

        // The Alpha vir-vir block
        int nVirOrbs = _mopi[h] - nOccOrbs;
        if(nVirOrbs){
            double **F = block_matrix(nVirOrbs, nVirOrbs);
            for(int i = 0; i < nVirOrbs; ++i){
                for(int j = 0; j <= i; ++j){
                    int ij = INDEX((i + moOffset + nOccOrbs), (j + moOffset + nOccOrbs));
                    F[i][j] = F[j][i] = temp[ij];
                }
            }
            double *evalPtr = &(evals[moOffset + nOccOrbs]);
            // Diagonalize the Fock matrix; on exit F is the rotation matrix to diagonalize F
            if(int stat = C_DSYEV('v','u', nVirOrbs, F[0], nVirOrbs, evalPtr, work, 3*_nmo)){
                fprintf(outfile,
                        "rotate(): Error in alpha Fvv[%1d] diagonalization. stat = %d\n", h, stat);
                throw PsiException("Semicanonicalization error", __FILE__, __LINE__);
            }
            // Now rotate the orbitals using the eigenvectors of F
            C_DGEMM('n', 't', _sopi[h], nVirOrbs, nVirOrbs, 1.0, &(C[h][0][nOccOrbs]),
                    _mopi[h], F[0], nVirOrbs, 0.0, &(_Ca[h][0][nOccOrbs]), _mopi[h]);
            // Update the Fock Matrix
            for(int i = 0; i < nVirOrbs; ++i){
                for(int j = 0; j <= i; ++j){
                    int ij = INDEX((i + moOffset + nOccOrbs), (j + moOffset + nOccOrbs));
                    temp[ij] = i == j ? evalPtr[i] : 0.0;
                }
            }
            free_block(F);
        }

        if(_print > 3){
            fprintf(outfile, "Semicanonical alpha orbitals for irrep %d\n", h);
            eivout(_Ca[h], &(evals[moOffset]), _sopi[h], _mopi[h], outfile);
        }
        moOffset += _mopi[h];
    }
    // Write the Fock matrix, in the alpha semicanonical basis, to disk
    if(_print > 3){
        fprintf(outfile, "The alpha Fock matrix in the semicanonical basis\n");
        print_array(temp, _nmo, outfile);
    }
    // Dump the new eigenvalues to the checkpoint file.
    _chkpt->wt_alpha_evals(evals);

    // Cb was never allocated in the moinfo routine, time to fix that
    _Cb = new double**[_nirreps];

    for(int n = 0; n < _nTriMo; ++n) temp[n] = 0.0;
    // The beta MOs - the Fock matrix is on disk from the so TEI sort routine
    if(_print > 3) fprintf(outfile, "The beta Fock matrix before semicanonicalization\n");
    IWL::read_one(_psio.get(), PSIF_OEI, PSIF_MO_B_FOCK, temp, _nTriMo, 0, _print > 3, outfile);
    for(int h = 0, moOffset = 0; h < _nirreps; ++h){
        if(_mopi[h] == 0) continue;
        _Cb[h] = block_matrix(_sopi[h], _mopi[h]);
        // The Beta occ-occ block
        int nOccOrbs = _clsdpi[h];
        if(nOccOrbs){
            double **F = block_matrix(nOccOrbs, nOccOrbs);
            for(int i = 0; i < nOccOrbs; ++i){
                for(int j = 0; j <= i; ++j){
                    int ij = INDEX((i + moOffset), (j + moOffset));
                    F[i][j] = F[j][i] = temp[ij];
                }
            }
            double *evalPtr = &(evals[moOffset]);
            // Diagonalize the Fock matrix; on exit F is the rotation matrix to diagonalize F
            if(int stat = C_DSYEV('v','u', nOccOrbs, F[0], nOccOrbs, evalPtr, work, 3*_nmo)){
                fprintf(outfile,
                        "rotate(): Error in beta Foo[%1d] diagonalization. stat = %d\n", h, stat);
                throw PsiException("Semicanonicalization error", __FILE__, __LINE__);
            }
            // Now rotate the orbitals using the eigenvectors of F
            C_DGEMM('n', 't', _sopi[h], nOccOrbs, nOccOrbs, 1.0, C[h][0],
                    _mopi[h], F[0], nOccOrbs, 0.0, _Cb[h][0], _mopi[h]);
            // Update the Fock Matrix
            for(int i = 0; i < nOccOrbs; ++i){
                for(int j = 0; j <= i; ++j){
                    int ij = INDEX((i + moOffset), (j + moOffset));
                    temp[ij] = i == j ? evalPtr[i] : 0.0;
                }
            }
            free_block(F);
        }

        // The Beta vir-vir block
        int nVirOrbs = _mopi[h] - nOccOrbs;
        if(nVirOrbs){
            double **F = block_matrix(nVirOrbs, nVirOrbs);
            for(int i = 0; i < nVirOrbs; ++i){
                for(int j = 0; j <= i; ++j){
                    int ij = INDEX((i + moOffset + nOccOrbs), (j + moOffset + nOccOrbs));
                    F[i][j] = F[j][i] = temp[ij];
                }
            }
            double *evalPtr = &(evals[moOffset + nOccOrbs]);
            // Diagonalize the Fock matrix; on exit F is the rotation matrix to diagonalize F
            if(int stat = C_DSYEV('v','u', nVirOrbs, F[0], nVirOrbs, evalPtr, work, 3*_nmo)) {
                fprintf(outfile, "rotate(): Error in beta Fvv[%1d] diagonalization. stat = %d\n", h, stat);
                throw PsiException("Semicanonicalization error", __FILE__, __LINE__);
            }
            // Now rotate the orbitals using the eigenvectors of F
            C_DGEMM('n', 't', _sopi[h], nVirOrbs, nVirOrbs, 1.0, &(C[h][0][nOccOrbs]),
                    _mopi[h], F[0], nVirOrbs, 0.0, &(_Cb[h][0][nOccOrbs]), _mopi[h]);
            // Update the Fock Matrix
            for(int i = 0; i < nVirOrbs; ++i){
                for(int j = 0; j <= i; ++j){
                    int ij = INDEX((i + moOffset + nOccOrbs), (j + moOffset + nOccOrbs));
                    temp[ij] = i == j ? evalPtr[i] : 0.0;
                }
            }
            free_block(F);
        }

        // We're done with C now
        free_block(C[h]);

        if(_print > 3){
            fprintf(outfile, "Semicanonical beta orbitals for irrep %d\n", h);
            eivout(_Cb[h], &(evals[moOffset]), _sopi[h], _mopi[h], outfile);
        }
        moOffset += _mopi[h];
    }
    // Dump the new eigenvalues to the checkpoint file.
    _chkpt->wt_beta_evals(evals);

    // Write the Fock matrix, in the beta semicanonical basis, to disk
    if(_print > 3){
        fprintf(outfile, "The beta Fock matrix in the semicanonical basis\n");
        print_array(temp, _nmo, outfile);
    }
//    IWL::write_one(_psio, PSIF_OEI, PSIF_SC_B_FOCK, _nTriMo, temp);

    delete [] C;
    free(work);
    free(evals);
    free(temp);

    /* The final stage is to copy the eigenvectors into a large array, and dump them
     * to the checkpoint file.  The is because the irrep-by-irrep scf_write function
     * requires the alpha and beta eigenvectors to exist in the checkpoint file already
     * which, of course, they don't for ROHF references.
     */
    double **fullC = block_matrix(_nso, _nmo);
    // The alpha eigenvectors
    for(int h = 0, soOffset = 0, moOffset = 0; h < _nirreps; ++h){
        for(int mu = 0; mu < _sopi[h]; ++mu){
            for(int p = 0; p < _mopi[h]; ++p){
                fullC[mu + soOffset][p + moOffset] = _Ca[h][mu][p];
            }
        }
        soOffset += _sopi[h];
        moOffset += _mopi[h];
    }
    _chkpt->wt_alpha_scf(fullC);
    // The beta eigenvectors
    for(int h = 0, soOffset = 0, moOffset = 0; h < _nirreps; ++h){
        for(int mu = 0; mu < _sopi[h]; ++mu){
            for(int p = 0; p < _mopi[h]; ++p){
                fullC[mu + soOffset][p + moOffset] = _Cb[h][mu][p];
            }
        }
        soOffset += _sopi[h];
        moOffset += _mopi[h];
    }
    _chkpt->wt_beta_scf(fullC);
    free_block(fullC);
}
