#include "integraltransform.h"
#include <libchkpt/chkpt.hpp>
#include <libpsio/psio.hpp>
#include <libciomr/libciomr.h>
#include <libiwl/iwl.hpp>
#include <libqt/qt.h>
#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include "psifiles.h"
#include "ccfiles.h"
#include "mospace.h"
#define EXTERN
#include <libdpd/dpd.gbl>

using namespace psi;

void
IntegralTransform::transform_tei_second_half(const shared_ptr<MOSpace> s1, const shared_ptr<MOSpace> s2,
                                             const shared_ptr<MOSpace> s3, const shared_ptr<MOSpace> s4)
{
    char *label = new char[100];

    // Grab the transformation coefficients
    double ***c3a = _aMOCoefficients[s3->label()];
    double ***c3b = _bMOCoefficients[s3->label()];
    double ***c4a = _aMOCoefficients[s4->label()];
    double ***c4b = _bMOCoefficients[s4->label()];
    // And the number of orbitals per irrep
    int *aOrbsPI3 = _aOrbsPI[s3->label()];
    int *bOrbsPI3 = _bOrbsPI[s3->label()];
    int *aOrbsPI4 = _aOrbsPI[s4->label()];
    int *bOrbsPI4 = _bOrbsPI[s4->label()];
    // The reindexing arrays
    int *aIndex1 = _aIndices[s1->label()];
    int *bIndex1 = _bIndices[s1->label()];
    int *aIndex2 = _aIndices[s2->label()];
    int *bIndex2 = _bIndices[s2->label()];
    int *aIndex3 = _aIndices[s3->label()];
    int *bIndex3 = _bIndices[s3->label()];
    int *aIndex4 = _aIndices[s4->label()];
    int *bIndex4 = _bIndices[s4->label()];

    // Grab control of DPD for now, but store the active number to restore it later
    int currentActiveDPD = psi::dpd_default;
    dpd_set_default(_myDPDNum);

    IWL *iwl;
    if(_useIWL) iwl = new IWL;
    int nBuckets;
    int thisBucketRows;
    size_t rowsPerBucket;
    size_t rowsLeft;
    size_t memFree;
    dpdbuf4 J, K;

    double **TMP = block_matrix(_nso, _nso);

    if(_print) {
        if(_transformationType == Restricted){
            fprintf(outfile, "\tStarting second half-transformation.\n");
        }else{
            fprintf(outfile, "\tStarting AA second half-transformation.\n");
        }
        fflush(outfile);
    }

    if(_useIWL) iwl = new IWL(_psio.get(), _iwlAAIntFile, _tolerance, 0, 0);

    _psio->open(_dpdIntFile, PSIO_OPEN_OLD);
    _psio->open(_aHtIntFile, PSIO_OPEN_OLD);

    int braCore = DPD_ID(s1, s2, Alpha, true);
    int braDisk = DPD_ID(s1, s2, Alpha, true);
    int ketCore = 0;
    int ketDisk = 3;
    sprintf(label, "Half-Transformed Ints (%c%c|nn)", toupper(s1->label()), toupper(s2->label()));
    dpd_buf4_init(&J, _aHtIntFile, 0, braCore, ketCore, braDisk, ketDisk, 0, label);
    if(_print > 5)
        fprintf(outfile, "Initializing %s, in core:(%d|%d) on disk(%d|%d)\n",
                            label, braCore, ketCore, braDisk, ketDisk);

    braCore = DPD_ID(s1, s2, Alpha, true);
    ketCore = DPD_ID(s3, s4, Alpha, false);
    braDisk = DPD_ID(s1, s2, Alpha, true);
    ketDisk = DPD_ID(s3, s4, Alpha, true);
    sprintf(label, "MO Ints (%c%c|%c%c)", toupper(s1->label()), toupper(s2->label()),
                                          toupper(s3->label()), toupper(s4->label()));
    dpd_buf4_init(&K, _dpdIntFile, 0, braCore, ketCore, braDisk, ketDisk, 0, label);
    if(_print > 5)
        fprintf(outfile, "Initializing %s, in core:(%d|%d) on disk(%d|%d)\n",
                            label, braCore, ketCore, braDisk, ketDisk);

    for(int h=0; h < _nirreps; h++) {
        if(J.params->coltot[h] && J.params->rowtot[h]) {
            memFree = static_cast<size_t>(dpd_memfree() - J.params->coltot[h] - K.params->coltot[h]);
            rowsPerBucket = memFree/(2 * J.params->coltot[h]);
            if(rowsPerBucket > J.params->rowtot[h])
                rowsPerBucket = static_cast<size_t>(J.params->rowtot[h]);
            nBuckets = static_cast<int>(ceil(static_cast<double>(J.params->rowtot[h])/
                                        static_cast<double>(rowsPerBucket)));
            rowsLeft = static_cast<size_t>(J.params->rowtot[h] % rowsPerBucket);
        }else {
            nBuckets = 0;
            rowsPerBucket = 0;
            rowsLeft = 0;
        }

        if(_print > 1) {
            fprintf(outfile, "\th = %d; memfree         = %lu\n", h, memFree);
            fprintf(outfile, "\th = %d; rows_per_bucket = %lu\n", h, rowsPerBucket);
            fprintf(outfile, "\th = %d; rows_left       = %lu\n", h, rowsLeft);
            fprintf(outfile, "\th = %d; nbuckets        = %d\n", h, nBuckets);
            fflush(outfile);
        }

        dpd_buf4_mat_irrep_init_block(&J, h, rowsPerBucket);
        dpd_buf4_mat_irrep_init_block(&K, h, rowsPerBucket);

        for(int n=0; n < nBuckets; n++) {
            if(nBuckets == 1)
                thisBucketRows = rowsPerBucket;
            else
                thisBucketRows = (n < nBuckets-1) ? rowsPerBucket : rowsLeft;
            dpd_buf4_mat_irrep_rd_block(&J, h, n*rowsPerBucket, thisBucketRows);
            for(int pq=0; pq < thisBucketRows; pq++) {
                for(int Gr=0; Gr < _nirreps; Gr++) {
                    // Transform ( S1 S2 | n n ) -> ( S1 S2 | n S4 )
                    int Gs = h^Gr;
                    int nrows = _sopi[Gr];
                    int ncols = aOrbsPI4[Gs];
                    int nlinks = _sopi[Gs];
                    int rs = J.col_offset[h][Gr];
                    if(nrows && ncols && nlinks)
                        C_DGEMM('n', 'n', nrows, ncols, nlinks, 1.0, &J.matrix[h][pq][rs],
                                nlinks, c4a[Gs][0], ncols, 0.0, TMP[0], _nso);
                    //TODO else if s4->label() == MOSPACE_NIL, copy buffer...

                    // Transform ( S1 S2 | n S4 ) -> ( S1 S2 | S3 S4 )
                    nrows = aOrbsPI3[Gr];
                    ncols = aOrbsPI4[Gs];
                    nlinks = _sopi[Gr];
                    rs = K.col_offset[h][Gr];
                    if(nrows && ncols && nlinks)
                        C_DGEMM('t', 'n', nrows, ncols, nlinks, 1.0, c3a[Gr][0], nrows ,
                                TMP[0], _nso, 0.0, &K.matrix[h][pq][rs], ncols);
                    //TODO else if s3->label() == MOSPACE_NIL, copy buffer...
                } /* Gr */
                if(_useIWL){
                    int p = aIndex1[K.params->roworb[h][pq+n*rowsPerBucket][0]];
                    int q = aIndex2[K.params->roworb[h][pq+n*rowsPerBucket][1]];
                    size_t PQ = INDEX(p,q);
                    for(int rs=0; rs < K.params->coltot[h]; rs++) {
                        int r = aIndex3[K.params->colorb[h][rs][0]];
                        int s = aIndex4[K.params->colorb[h][rs][1]];
                        size_t RS = INDEX(r,s);
                        if(r >= s && RS <= PQ)
                            iwl->write_value(p, q, r, s, K.matrix[h][pq][rs],
                                             _printTei, outfile, 0);
                    } /* rs */
                }
            } /* pq */
            dpd_buf4_mat_irrep_wrt_block(&K, h, n*rowsPerBucket, thisBucketRows);
        }
        dpd_buf4_mat_irrep_close_block(&J, h, rowsPerBucket);
        dpd_buf4_mat_irrep_close_block(&K, h, rowsPerBucket);
    }
    dpd_buf4_close(&K);
    dpd_buf4_close(&J);

    if(_useIWL){
        iwl->flush(1);
        iwl->set_keep_flag(1);
        // This closes the file too
        delete iwl;
    }

    if(_transformationType != Restricted){
        if(_print) {
            fprintf(outfile, "\tStarting AB second half-transformation.\n");
            fflush(outfile);
        }
        if(_useIWL) iwl = new IWL(_psio.get(), _iwlABIntFile, _tolerance, 0, 0);

        braCore = braDisk = DPD_ID(s1, s2, Alpha, true);
        ketCore = 0;
        ketDisk = 3;
        sprintf(label, "Half-Transformed Ints (%c%c|nn)", toupper(s1->label()), toupper(s2->label()));
        dpd_buf4_init(&J, _aHtIntFile, 0, braCore, ketCore, braDisk, ketDisk, 0, label);
        if(_print > 5)
            fprintf(outfile, "Initializing %s, in core:(%d|%d) on disk(%d|%d)\n",
                                label, braCore, ketCore, braDisk, ketDisk);

        braCore = DPD_ID(s1, s2, Alpha, true);
        ketCore = DPD_ID(s3, s4, Beta,  false);
        braDisk = DPD_ID(s1, s2, Alpha, true);
        ketDisk = DPD_ID(s3, s4, Beta,  true);
        sprintf(label, "MO Ints (%c%c|%c%c)", toupper(s1->label()), toupper(s2->label()),
                                              tolower(s3->label()), tolower(s4->label()));
        dpd_buf4_init(&K, _dpdIntFile, 0, braCore, ketCore, braDisk, ketDisk, 0, label);
        if(_print > 5)
            fprintf(outfile, "Initializing %s, in core:(%d|%d) on disk(%d|%d)\n",
                                label, braCore, ketCore, braDisk, ketDisk);

        for(int h=0; h < _nirreps; h++) {
            if(J.params->coltot[h] && J.params->rowtot[h]) {
                memFree = static_cast<size_t>(dpd_memfree() - J.params->coltot[h] - K.params->coltot[h]);
                rowsPerBucket = memFree/(2 * J.params->coltot[h]);
                if(rowsPerBucket > J.params->rowtot[h])
                    rowsPerBucket = static_cast<size_t>(J.params->rowtot[h]);
                nBuckets = static_cast<int>(ceil(static_cast<double>(J.params->rowtot[h])/
                        static_cast<double>(rowsPerBucket)));
                rowsLeft = static_cast<size_t>(J.params->rowtot[h] % rowsPerBucket);
            }else{
                nBuckets = 0;
                rowsPerBucket = 0;
                rowsLeft = 0;
            }

            if(_print > 1) {
                fprintf(outfile, "\th = %d; memfree         = %lu\n", h, memFree);
                fprintf(outfile, "\th = %d; rows_per_bucket = %lu\n", h, rowsPerBucket);
                fprintf(outfile, "\th = %d; rows_left       = %lu\n", h, rowsLeft);
                fprintf(outfile, "\th = %d; nbuckets        = %d\n", h, nBuckets);
                fflush(outfile);
            }

            dpd_buf4_mat_irrep_init_block(&J, h, rowsPerBucket);
            dpd_buf4_mat_irrep_init_block(&K, h, rowsPerBucket);

            for(int n=0; n < nBuckets; n++){
                if(nBuckets == 1)
                    thisBucketRows = rowsPerBucket;
                else
                    thisBucketRows = (n < nBuckets-1) ? rowsPerBucket : rowsLeft;
                dpd_buf4_mat_irrep_rd_block(&J, h, n*rowsPerBucket, thisBucketRows);
                for(int pq=0; pq < thisBucketRows; pq++) {
                    for(int Gr=0; Gr < _nirreps; Gr++) {
                        // Transform ( S1 S2 | n n ) -> ( S1 S2 | n s4 )
                        int Gs = h^Gr;
                        int nrows = _sopi[Gr];
                        int ncols = bOrbsPI4[Gs];
                        int nlinks = _sopi[Gs];
                        int rs = J.col_offset[h][Gr];
                        if(nrows && ncols && nlinks)
                            C_DGEMM('n', 'n', nrows, ncols, nlinks, 1.0, &J.matrix[h][pq][rs],
                                    nlinks, c4b[Gs][0], ncols, 0.0, TMP[0], _nso);
                        //TODO else if s4->label() == MOSPACE_NIL, copy buffer...

                        // Transform ( S1 S2 | n s4 ) -> ( S1 S2 | s3 s4 )
                        nrows = bOrbsPI3[Gr];
                        ncols = bOrbsPI4[Gs];
                        nlinks = _sopi[Gr];
                        rs = K.col_offset[h][Gr];
                        if(nrows && ncols && nlinks)
                            C_DGEMM('t', 'n', nrows, ncols, nlinks, 1.0, c3b[Gr][0], nrows,
                                    TMP[0], _nso, 0.0, &K.matrix[h][pq][rs], ncols);
                        //TODO else if s3->label() == MOSPACE_NIL, copy buffer...
                    } /* Gr */
                    if(_useIWL){
                        int p = aIndex1[K.params->roworb[h][pq+n*rowsPerBucket][0]];
                        int q = aIndex2[K.params->roworb[h][pq+n*rowsPerBucket][1]];
                        size_t PQ = INDEX(p,q);
                        for(int rs=0; rs < K.params->coltot[h]; rs++) {
                            int r = bIndex3[K.params->colorb[h][rs][0]];
                            int s = bIndex4[K.params->colorb[h][rs][1]];
                            size_t RS = INDEX(r,s);
                            if(r >= s)
                                iwl->write_value(p, q, r, s, K.matrix[h][pq][rs],
                                                 _printTei, outfile, 0);
                        } /* rs */
                    }
                } /* pq */
                dpd_buf4_mat_irrep_wrt_block(&K, h, n*rowsPerBucket, thisBucketRows);
            }
            dpd_buf4_mat_irrep_close_block(&J, h, rowsPerBucket);
            dpd_buf4_mat_irrep_close_block(&K, h, rowsPerBucket);
        }
        dpd_buf4_close(&K);
        dpd_buf4_close(&J);

        if(_useIWL){
            iwl->flush(1);
            iwl->set_keep_flag(1);
            // This closes the file too
            delete iwl;
        }

        /*** AA/AB two-electron integral transformation complete ***/

        if(_print) {
            fprintf(outfile, "\tStarting BB second half-transformation.\n");
            fflush(outfile);
        }
        if(_useIWL) iwl = new IWL(_psio.get(), _iwlBBIntFile, _tolerance, 0, 0);

        _psio->open(_bHtIntFile, PSIO_OPEN_OLD);

        braCore = DPD_ID(s1, s2, Beta, true);
        ketCore = 0;
        braDisk = DPD_ID(s1, s2, Beta, true);
        ketDisk = 3;
        sprintf(label, "Half-Transformed Ints (%c%c|nn)", tolower(s1->label()), tolower(s2->label()));
        dpd_buf4_init(&J, _bHtIntFile, 0, braCore, ketCore, braDisk, ketDisk, 0, label);
        if(_print > 5)
            fprintf(outfile, "Initializing %s, in core:(%d|%d) on disk(%d|%d)\n",
                                label, braCore, ketCore, braDisk, ketDisk);

        braCore = DPD_ID(s1, s2, Beta, true);
        ketCore = DPD_ID(s3, s4, Beta, false);
        braDisk = DPD_ID(s1, s2, Beta, true);
        ketDisk = DPD_ID(s3, s4, Beta, true);
        sprintf(label, "MO Ints (%c%c|%c%c)", tolower(s1->label()), tolower(s2->label()),
                                              tolower(s3->label()), tolower(s4->label()));
        dpd_buf4_init(&K, _dpdIntFile, 0, braCore, ketCore, braDisk, ketDisk, 0, label);
        if(_print > 5)
            fprintf(outfile, "Initializing %s, in core:(%d|%d) on disk(%d|%d)\n",
                                label, braCore, ketCore, braDisk, ketDisk);

        for(int h=0; h < _nirreps; h++) {
            if (J.params->coltot[h] && J.params->rowtot[h]) {
                memFree = static_cast<size_t>(dpd_memfree() - J.params->coltot[h] - K.params->coltot[h]);
                rowsPerBucket = memFree/(2 * J.params->coltot[h]);
                if(rowsPerBucket > J.params->rowtot[h])
                    rowsPerBucket = static_cast<size_t>(J.params->rowtot[h]);
                nBuckets = static_cast<int>(ceil(static_cast<double>(J.params->rowtot[h])/
                                static_cast<double>(rowsPerBucket)));
                rowsLeft = static_cast<size_t>(J.params->rowtot[h] % rowsPerBucket);
            }
            else {
                nBuckets = 0;
                rowsPerBucket = 0;
                rowsLeft = 0;
            }

            if(_print > 1) {
                fprintf(outfile, "\th = %d; memfree         = %lu\n", h, memFree);
                fprintf(outfile, "\th = %d; rows_per_bucket = %lu\n", h, rowsPerBucket);
                fprintf(outfile, "\th = %d; rows_left       = %lu\n", h, rowsLeft);
                fprintf(outfile, "\th = %d; nbuckets        = %d\n", h, nBuckets);
                fflush(outfile);
            }

            dpd_buf4_mat_irrep_init_block(&J, h, rowsPerBucket);
            dpd_buf4_mat_irrep_init_block(&K, h, rowsPerBucket);

            for(int n=0; n < nBuckets; n++) {
                if(nBuckets == 1)
                    thisBucketRows = rowsPerBucket;
                else
                    thisBucketRows = (n < nBuckets-1) ? rowsPerBucket : rowsLeft;
                dpd_buf4_mat_irrep_rd_block(&J, h, n*rowsPerBucket, thisBucketRows);
                for(int pq=0; pq < thisBucketRows; pq++) {
                    for(int Gr=0; Gr < _nirreps; Gr++) {
                        // Transform ( s1 s2 | n n ) -> ( s1 s2 | n s4 )
                        int Gs = h^Gr;
                        int nrows = _sopi[Gr];
                        int ncols = bOrbsPI4[Gs];
                        int nlinks = _sopi[Gs];
                        int rs = J.col_offset[h][Gr];
                        if(nrows && ncols && nlinks)
                            C_DGEMM('n', 'n', nrows, ncols, nlinks, 1.0, &J.matrix[h][pq][rs],
                                    nlinks, c4b[Gs][0], ncols, 0.0, TMP[0], _nso);

                        // Transform ( s1 s2 | n s4 ) -> ( s1 s2 | s3 s4 )
                        nrows = bOrbsPI3[Gr];
                        ncols = bOrbsPI4[Gs];
                        nlinks = _sopi[Gr];
                        rs = K.col_offset[h][Gr];
                        if(nrows && ncols && nlinks)
                            C_DGEMM('t', 'n', nrows, ncols, nlinks, 1.0, c3b[Gr][0], nrows,
                                    TMP[0], _nso, 0.0, &K.matrix[h][pq][rs], ncols);
                    } /* Gr */
                    if(_useIWL){
                        int p = bIndex1[K.params->roworb[h][pq+n*rowsPerBucket][0]];
                        int q = bIndex2[K.params->roworb[h][pq+n*rowsPerBucket][1]];
                        size_t PQ = INDEX(p,q);
                        for(int rs=0; rs < K.params->coltot[h]; rs++) {
                            int r = bIndex3[K.params->colorb[h][rs][0]];
                            int s = bIndex4[K.params->colorb[h][rs][1]];
                            size_t RS = INDEX(r,s);
                            if(r >= s && RS <= PQ)
                                iwl->write_value(p, q, r, s, K.matrix[h][pq][rs],
                                                 _printTei, outfile, 0);
                        } /* rs */
                    }
                } /* pq */
                dpd_buf4_mat_irrep_wrt_block(&K, h, n*rowsPerBucket, thisBucketRows);
            }
            dpd_buf4_mat_irrep_close_block(&J, h, rowsPerBucket);
            dpd_buf4_mat_irrep_close_block(&K, h, rowsPerBucket);
        }
        dpd_buf4_close(&K);
        dpd_buf4_close(&J);

        _psio->close(_bHtIntFile, _keepHtInts);

        if(_useIWL){
            iwl->flush(1);
            iwl->set_keep_flag(1);
            // This closes the file too
            delete iwl;
        }
        /*** BB two-electron integral transformation complete ***/
    } // End "if not restricted transformation"


    _psio->close(_dpdIntFile, 1);
    _psio->close(_aHtIntFile, _keepHtInts);

    free_block(TMP);
    delete [] label;

    if(_print){
        fprintf(outfile, "\tTwo-electron integral transformation complete.\n");
        fflush(outfile);
    }

    // Hand DPD control back to the user
    dpd_set_default(currentActiveDPD);
}
