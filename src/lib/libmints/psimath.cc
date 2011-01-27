#include <libqt/qt.h>
#include <libmints/matrix.h>
#include <libmints/vector.h>
#include <psi4-dec.h>
#include <libmints/psimath.h>

namespace psi {

/// PSI_DGBMV, a wrapper to C_DGBMV using objects
void PSI_DGBMV(int irrep, char trans, int m, int n, int kl, int ku, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> x, int incx, double beta, shared_ptr<Vector> y, int incy)
{
    C_DGBMV(trans, m, n, kl, ku, alpha, a->get_pointer(irrep)[0], lda, x->get_pointer(irrep), incx, beta, y->get_pointer(irrep), incy);
}
/// PSI_DGEMM, a wrapper to C_DGEMM using objects
void PSI_DGEMM(int irrep, char transa, char transb, int m, int n, int k, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, double beta, shared_ptr<Matrix> c, int ldc)
{
    C_DGEMM(transa, transb, m, n, k, alpha, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, beta, c->get_pointer(irrep)[0], ldc);
}
/// PSI_DGEMV, a wrapper to C_DGEMV using objects
void PSI_DGEMV(int irrep, char trans, int m, int n, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> x, int incx, double beta, shared_ptr<Vector> y, int incy)
{
    C_DGEMV(trans, m, n, alpha, a->get_pointer(irrep)[0], lda, x->get_pointer(irrep), incx, beta, y->get_pointer(irrep), incy);
}
/// PSI_DGER, a wrapper to C_DGER using objects
void PSI_DGER(int irrep, int m, int n, double alpha, shared_ptr<Vector> x, int incx, shared_ptr<Vector> y, int incy, shared_ptr<Matrix> a, int lda)
{
    C_DGER(m, n, alpha, x->get_pointer(irrep), incx, y->get_pointer(irrep), incy, a->get_pointer(irrep)[0], lda);
}
/// PSI_DSBMV, a wrapper to C_DSBMV using objects
void PSI_DSBMV(int irrep, char uplo, int n, int k, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> x, int incx, double beta, shared_ptr<Vector> y, int incy)
{
    C_DSBMV(uplo, n, k, alpha, a->get_pointer(irrep)[0], lda, x->get_pointer(irrep), incx, beta, y->get_pointer(irrep), incy);
}
/// PSI_DSYMM, a wrapper to C_DSYMM using objects
void PSI_DSYMM(int irrep, char side, char uplo, int m, int n, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, double beta, shared_ptr<Matrix> c, int ldc)
{
    C_DSYMM(side, uplo, m, n, alpha, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, beta, c->get_pointer(irrep)[0], ldc);
}
/// PSI_DSYMV, a wrapper to C_DSYMV using objects
void PSI_DSYMV(int irrep, char uplo, int n, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> x, int incx, double beta, shared_ptr<Vector> y, int incy)
{
    C_DSYMV(uplo, n, alpha, a->get_pointer(irrep)[0], lda, x->get_pointer(irrep), incx, beta, y->get_pointer(irrep), incy);
}
/// PSI_DSYR, a wrapper to C_DSYR using objects
void PSI_DSYR(int irrep, char uplo, int n, double alpha, shared_ptr<Vector> x, int incx, shared_ptr<Matrix> a, int lda)
{
    C_DSYR(uplo, n, alpha, x->get_pointer(irrep), incx, a->get_pointer(irrep)[0], lda);
}
/// PSI_DSYR2, a wrapper to C_DSYR2 using objects
void PSI_DSYR2(int irrep, char uplo, int n, double alpha, shared_ptr<Vector> x, int incx, shared_ptr<Vector> y, int incy, shared_ptr<Matrix> a, int lda)
{
    C_DSYR2(uplo, n, alpha, x->get_pointer(irrep), incx, y->get_pointer(irrep), incy, a->get_pointer(irrep)[0], lda);
}
/// PSI_DSYR2K, a wrapper to C_DSYR2K using objects
void PSI_DSYR2K(int irrep, char uplo, char trans, int n, int k, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, double beta, shared_ptr<Matrix> c, int ldc)
{
    C_DSYR2K(uplo, trans, n, k, alpha, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, beta, c->get_pointer(irrep)[0], ldc);
}
/// PSI_DSYRK, a wrapper to C_DSYRK using objects
void PSI_DSYRK(int irrep, char uplo, char trans, int n, int k, double alpha, shared_ptr<Matrix> a, int lda, double beta, shared_ptr<Matrix> c, int ldc)
{
    C_DSYRK(uplo, trans, n, k, alpha, a->get_pointer(irrep)[0], lda, beta, c->get_pointer(irrep)[0], ldc);
}
/// PSI_DTBMV, a wrapper to C_DTBMV using objects
void PSI_DTBMV(int irrep, char uplo, char trans, char diag, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> x, int incx)
{
    C_DTBMV(uplo, trans, diag, n, k, a->get_pointer(irrep)[0], lda, x->get_pointer(irrep), incx);
}
/// PSI_DTBSV, a wrapper to C_DTBSV using objects
void PSI_DTBSV(int irrep, char uplo, char trans, char diag, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> x, int incx)
{
    C_DTBSV(uplo, trans, diag, n, k, a->get_pointer(irrep)[0], lda, x->get_pointer(irrep), incx);
}
/// PSI_DTRMM, a wrapper to C_DTRMM using objects
void PSI_DTRMM(int irrep, char side, char uplo, char transa, char diag, int m, int n, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb)
{
    C_DTRMM(side, uplo, transa, diag, m, n, alpha, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DTRMV, a wrapper to C_DTRMV using objects
void PSI_DTRMV(int irrep, char uplo, char trans, char diag, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> x, int incx)
{
    C_DTRMV(uplo, trans, diag, n, a->get_pointer(irrep)[0], lda, x->get_pointer(irrep), incx);
}
/// PSI_DTRSM, a wrapper to C_DTRSM using objects
void PSI_DTRSM(int irrep, char side, char uplo, char transa, char diag, int m, int n, double alpha, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb)
{
    C_DTRSM(side, uplo, transa, diag, m, n, alpha, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DTRSV, a wrapper to C_DTRSV using objects
void PSI_DTRSV(int irrep, char uplo, char trans, char diag, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> x, int incx)
{
    C_DTRSV(uplo, trans, diag, n, a->get_pointer(irrep)[0], lda, x->get_pointer(irrep), incx);
}

/// PSI_DROT, a wrapper to C_DROT using objects
void PSI_DROT(int irrep, unsigned long int n, shared_ptr<Vector> x, int incx, shared_ptr<Vector> y, int incy, double c, double s)
{
    C_DROT(n, x->get_pointer(irrep), incx, y->get_pointer(irrep), incy, c, s);
}
/// PSI_DSWAP, a wrapper to C_DSWAP using objects
void PSI_DSWAP(int irrep, unsigned long int n, shared_ptr<Vector> x, int incx, shared_ptr<Vector> y, int incy)
{
    C_DSWAP(n, x->get_pointer(irrep), incx, y->get_pointer(irrep), incy);
}
/// PSI_DCOPY, a wrapper to C_DCOPY using objects
void PSI_DCOPY(int irrep, unsigned long int n, shared_ptr<Vector> x, int incx, shared_ptr<Vector> y, int incy)
{
    C_DCOPY(n, x->get_pointer(irrep), incx, y->get_pointer(irrep), incy);
}
/// PSI_DSCAL, a wrapper to C_DSCAL using objects
void PSI_DSCAL(int irrep, unsigned long int n, double alpha, shared_ptr<Vector> x, int incx)
{
    C_DSCAL(n, alpha, x->get_pointer(irrep), incx);
}
/// PSI_DAXPY, a wrapper to C_DAXPY using objects
void PSI_DAXPY(int irrep, unsigned long int n, double alpha, shared_ptr<Vector> x, int incx, shared_ptr<Vector> y, int incy)
{
    C_DAXPY(n, alpha, x->get_pointer(irrep), incx, y->get_pointer(irrep), incy);
}
/// PSI_DDOT, a wrapper to C_DDOT using objects
double PSI_DDOT(int irrep, unsigned long int n, shared_ptr<Vector> x, int incx, shared_ptr<Vector> y, int incy)
{
    return C_DDOT(n, x->get_pointer(irrep), incx, y->get_pointer(irrep), incy);
}
/// PSI_DNRM2, a wrapper to C_DNRM2 using objects
double PSI_DNRM2(int irrep, unsigned long int n, shared_ptr<Vector> x, int incx)
{
    return C_DNRM2(n, x->get_pointer(irrep), incx);
}
/// PSI_DASUM, a wrapper to C_DASUM using objects
double PSI_DASUM(int irrep, unsigned long int n, shared_ptr<Vector> x, int incx)
{
    return C_DASUM(n, x->get_pointer(irrep), incx);
}
/// PSI_IDAMAX, a wrapper to C_IDAMAX using objects
unsigned long int PSI_IDAMAX(int irrep, unsigned long int n, shared_ptr<Vector> x, int incx)
{
    return C_IDAMAX(n, x->get_pointer(irrep), incx);
}


/// LAPACK
/// PSI_DBDSDC, a wrapper to return C_DBDSDC using objects
int PSI_DBDSDC(int irrep, char uplo, char compq, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> u, int ldu, shared_ptr<Matrix> vt, int ldvt, shared_ptr<Vector> q, shared_ptr<IntVector> iq, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DBDSDC(uplo, compq, n, d->get_pointer(irrep), e->get_pointer(irrep), u->get_pointer(irrep)[0], ldu, vt->get_pointer(irrep)[0], ldvt, q->get_pointer(irrep), iq->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DBDSQR, a wrapper to return C_DBDSQR using objects
int PSI_DBDSQR(int irrep, char uplo, int n, int ncvt, int nru, int ncc, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> vt, int ldvt, shared_ptr<Matrix> u, int ldu, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work)
{
    return C_DBDSQR(uplo, n, ncvt, nru, ncc, d->get_pointer(irrep), e->get_pointer(irrep), vt->get_pointer(irrep)[0], ldvt, u->get_pointer(irrep)[0], ldu, c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep));
}
/// PSI_DDISNA, a wrapper to return C_DDISNA using objects
int PSI_DDISNA(int irrep, char job, int m, int n, shared_ptr<Vector> d, shared_ptr<Vector> sep)
{
    return C_DDISNA(job, m, n, d->get_pointer(irrep), sep->get_pointer(irrep));
}
/// PSI_DGBBRD, a wrapper to return C_DGBBRD using objects
int PSI_DGBBRD(int irrep, char vect, int m, int n, int ncc, int kl, int ku, shared_ptr<Matrix> ab, int ldab, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> q, int ldq, shared_ptr<Matrix> pt, int ldpt, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work)
{
    return C_DGBBRD(vect, m, n, ncc, kl, ku, ab->get_pointer(irrep)[0], ldab, d->get_pointer(irrep), e->get_pointer(irrep), q->get_pointer(irrep)[0], ldq, pt->get_pointer(irrep)[0], ldpt, c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep));
}
/// PSI_DGBCON, a wrapper to return C_DGBCON using objects
int PSI_DGBCON(int irrep, char norm, int n, int kl, int ku, shared_ptr<Matrix> ab, int ldab, shared_ptr<IntVector> ipiv, double anorm, shared_ptr<Vector> rcond, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGBCON(norm, n, kl, ku, ab->get_pointer(irrep)[0], ldab, ipiv->get_pointer(irrep), anorm, rcond->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGBEQU, a wrapper to return C_DGBEQU using objects
int PSI_DGBEQU(int irrep, int m, int n, int kl, int ku, shared_ptr<Matrix> ab, int ldab, shared_ptr<Vector> r, shared_ptr<Vector> c, shared_ptr<Vector> rowcnd, shared_ptr<Vector> colcnd, shared_ptr<Vector> amax)
{
    return C_DGBEQU(m, n, kl, ku, ab->get_pointer(irrep)[0], ldab, r->get_pointer(irrep), c->get_pointer(irrep), rowcnd->get_pointer(irrep), colcnd->get_pointer(irrep), amax->get_pointer(irrep));
}
/// PSI_DGBRFS, a wrapper to return C_DGBRFS using objects
int PSI_DGBRFS(int irrep, char trans, int n, int kl, int ku, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> afb, int ldafb, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGBRFS(trans, n, kl, ku, nrhs, ab->get_pointer(irrep)[0], ldab, afb->get_pointer(irrep)[0], ldafb, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGBSV, a wrapper to return C_DGBSV using objects
int PSI_DGBSV(int irrep, int n, int kl, int ku, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb)
{
    return C_DGBSV(n, kl, ku, nrhs, ab->get_pointer(irrep)[0], ldab, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DGBSVX, a wrapper to return C_DGBSVX using objects
int PSI_DGBSVX(int irrep, char fact, char trans, int n, int kl, int ku, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> afb, int ldafb, shared_ptr<IntVector> ipiv, char equed, shared_ptr<Vector> r, shared_ptr<Vector> c, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> rcond, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGBSVX(fact, trans, n, kl, ku, nrhs, ab->get_pointer(irrep)[0], ldab, afb->get_pointer(irrep)[0], ldafb, ipiv->get_pointer(irrep), equed, r->get_pointer(irrep), c->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, rcond->get_pointer(irrep), ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGBTRF, a wrapper to return C_DGBTRF using objects
int PSI_DGBTRF(int irrep, int m, int n, int kl, int ku, shared_ptr<Matrix> ab, int ldab, shared_ptr<IntVector> ipiv)
{
    return C_DGBTRF(m, n, kl, ku, ab->get_pointer(irrep)[0], ldab, ipiv->get_pointer(irrep));
}
/// PSI_DGBTRS, a wrapper to return C_DGBTRS using objects
int PSI_DGBTRS(int irrep, char trans, int n, int kl, int ku, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb)
{
    return C_DGBTRS(trans, n, kl, ku, nrhs, ab->get_pointer(irrep)[0], ldab, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DGEBAK, a wrapper to return C_DGEBAK using objects
int PSI_DGEBAK(int irrep, char job, char side, int n, int ilo, int ihi, shared_ptr<Vector> scale, int m, shared_ptr<Matrix> v, int ldv)
{
    return C_DGEBAK(job, side, n, ilo, ihi, scale->get_pointer(irrep), m, v->get_pointer(irrep)[0], ldv);
}
/// PSI_DGEBAL, a wrapper to return C_DGEBAL using objects
int PSI_DGEBAL(int irrep, char job, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ilo, shared_ptr<IntVector> ihi, shared_ptr<Vector> scale)
{
    return C_DGEBAL(job, n, a->get_pointer(irrep)[0], lda, ilo->get_pointer(irrep), ihi->get_pointer(irrep), scale->get_pointer(irrep));
}
/// PSI_DGEBRD, a wrapper to return C_DGEBRD using objects
int PSI_DGEBRD(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Vector> tauq, shared_ptr<Vector> taup, shared_ptr<Vector> work, int lwork)
{
    return C_DGEBRD(m, n, a->get_pointer(irrep)[0], lda, d->get_pointer(irrep), e->get_pointer(irrep), tauq->get_pointer(irrep), taup->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGECON, a wrapper to return C_DGECON using objects
int PSI_DGECON(int irrep, char norm, int n, shared_ptr<Matrix> a, int lda, double anorm, shared_ptr<Vector> rcond, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGECON(norm, n, a->get_pointer(irrep)[0], lda, anorm, rcond->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGEEQU, a wrapper to return C_DGEEQU using objects
int PSI_DGEEQU(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> r, shared_ptr<Vector> c, shared_ptr<Vector> rowcnd, shared_ptr<Vector> colcnd, shared_ptr<Vector> amax)
{
    return C_DGEEQU(m, n, a->get_pointer(irrep)[0], lda, r->get_pointer(irrep), c->get_pointer(irrep), rowcnd->get_pointer(irrep), colcnd->get_pointer(irrep), amax->get_pointer(irrep));
}
/// PSI_DGEES, a wrapper to return C_DGEES using objects
int PSI_DGEES(int irrep, char jobvs, char sort, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> sdim, shared_ptr<Vector> wr, shared_ptr<Vector> wi, shared_ptr<Matrix> vs, int ldvs, shared_ptr<Vector> work, int lwork)
{
    return C_DGEES(jobvs, sort, n, a->get_pointer(irrep)[0], lda, sdim->get_pointer(irrep), wr->get_pointer(irrep), wi->get_pointer(irrep), vs->get_pointer(irrep)[0], ldvs, work->get_pointer(irrep), lwork);
}
/// PSI_DGEESX, a wrapper to return C_DGEESX using objects
int PSI_DGEESX(int irrep, char jobvs, char sort, char sense, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> sdim, shared_ptr<Vector> wr, shared_ptr<Vector> wi, shared_ptr<Matrix> vs, int ldvs, shared_ptr<Vector> rconde, shared_ptr<Vector> rcondv, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DGEESX(jobvs, sort, sense, n, a->get_pointer(irrep)[0], lda, sdim->get_pointer(irrep), wr->get_pointer(irrep), wi->get_pointer(irrep), vs->get_pointer(irrep)[0], ldvs, rconde->get_pointer(irrep), rcondv->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DGEEV, a wrapper to return C_DGEEV using objects
int PSI_DGEEV(int irrep, char jobvl, char jobvr, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> wr, shared_ptr<Vector> wi, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, shared_ptr<Vector> work, int lwork)
{
    return C_DGEEV(jobvl, jobvr, n, a->get_pointer(irrep)[0], lda, wr->get_pointer(irrep), wi->get_pointer(irrep), vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, work->get_pointer(irrep), lwork);
}
/// PSI_DGEEVX, a wrapper to return C_DGEEVX using objects
int PSI_DGEEVX(int irrep, char balanc, char jobvl, char jobvr, char sense, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> wr, shared_ptr<Vector> wi, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, shared_ptr<IntVector> ilo, shared_ptr<IntVector> ihi, shared_ptr<Vector> scale, shared_ptr<Vector> abnrm, shared_ptr<Vector> rconde, shared_ptr<Vector> rcondv, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork)
{
    return C_DGEEVX(balanc, jobvl, jobvr, sense, n, a->get_pointer(irrep)[0], lda, wr->get_pointer(irrep), wi->get_pointer(irrep), vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, ilo->get_pointer(irrep), ihi->get_pointer(irrep), scale->get_pointer(irrep), abnrm->get_pointer(irrep), rconde->get_pointer(irrep), rcondv->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep));
}
/// PSI_DGEGS, a wrapper to return C_DGEGS using objects
int PSI_DGEGS(int irrep, char jobvsl, char jobvsr, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> alphar, shared_ptr<Vector> alphai, shared_ptr<Vector> beta, shared_ptr<Matrix> vsl, int ldvsl, shared_ptr<Matrix> vsr, int ldvsr, shared_ptr<Vector> work, int lwork)
{
    return C_DGEGS(jobvsl, jobvsr, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, alphar->get_pointer(irrep), alphai->get_pointer(irrep), beta->get_pointer(irrep), vsl->get_pointer(irrep)[0], ldvsl, vsr->get_pointer(irrep)[0], ldvsr, work->get_pointer(irrep), lwork);
}
/// PSI_DGEGV, a wrapper to return C_DGEGV using objects
int PSI_DGEGV(int irrep, char jobvl, char jobvr, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> alphar, shared_ptr<Vector> alphai, shared_ptr<Vector> beta, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, shared_ptr<Vector> work, int lwork)
{
    return C_DGEGV(jobvl, jobvr, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, alphar->get_pointer(irrep), alphai->get_pointer(irrep), beta->get_pointer(irrep), vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, work->get_pointer(irrep), lwork);
}
/// PSI_DGEHRD, a wrapper to return C_DGEHRD using objects
int PSI_DGEHRD(int irrep, int n, int ilo, int ihi, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DGEHRD(n, ilo, ihi, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGELQF, a wrapper to return C_DGELQF using objects
int PSI_DGELQF(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DGELQF(m, n, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGELS, a wrapper to return C_DGELS using objects
int PSI_DGELS(int irrep, char trans, int m, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> work, int lwork)
{
    return C_DGELS(trans, m, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, work->get_pointer(irrep), lwork);
}
/// PSI_DGELSD, a wrapper to return C_DGELSD using objects
int PSI_DGELSD(int irrep, int m, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> s, double rcond, shared_ptr<IntVector> rank, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork)
{
    return C_DGELSD(m, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, s->get_pointer(irrep), rcond, rank->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep));
}
/// PSI_DGELSS, a wrapper to return C_DGELSS using objects
int PSI_DGELSS(int irrep, int m, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> s, double rcond, shared_ptr<IntVector> rank, shared_ptr<Vector> work, int lwork)
{
    return C_DGELSS(m, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, s->get_pointer(irrep), rcond, rank->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGELSX, a wrapper to return C_DGELSX using objects
int PSI_DGELSX(int irrep, int m, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<IntVector> jpvt, double rcond, shared_ptr<IntVector> rank, shared_ptr<Vector> work)
{
    return C_DGELSX(m, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, jpvt->get_pointer(irrep), rcond, rank->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DGELSY, a wrapper to return C_DGELSY using objects
int PSI_DGELSY(int irrep, int m, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<IntVector> jpvt, double rcond, shared_ptr<IntVector> rank, shared_ptr<Vector> work, int lwork)
{
    return C_DGELSY(m, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, jpvt->get_pointer(irrep), rcond, rank->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGEQLF, a wrapper to return C_DGEQLF using objects
int PSI_DGEQLF(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DGEQLF(m, n, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGEQP3, a wrapper to return C_DGEQP3 using objects
int PSI_DGEQP3(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> jpvt, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DGEQP3(m, n, a->get_pointer(irrep)[0], lda, jpvt->get_pointer(irrep), tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGEQPF, a wrapper to return C_DGEQPF using objects
int PSI_DGEQPF(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> jpvt, shared_ptr<Vector> tau, shared_ptr<Vector> work)
{
    return C_DGEQPF(m, n, a->get_pointer(irrep)[0], lda, jpvt->get_pointer(irrep), tau->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DGERFS, a wrapper to return C_DGERFS using objects
int PSI_DGERFS(int irrep, char trans, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> af, int ldaf, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGERFS(trans, n, nrhs, a->get_pointer(irrep)[0], lda, af->get_pointer(irrep)[0], ldaf, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGERQF, a wrapper to return C_DGERQF using objects
int PSI_DGERQF(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DGERQF(m, n, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGESDD, a wrapper to return C_DGESDD using objects
int PSI_DGESDD(int irrep, char jobz, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> s, shared_ptr<Matrix> u, int ldu, shared_ptr<Matrix> vt, int ldvt, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork)
{
    return C_DGESDD(jobz, m, n, a->get_pointer(irrep)[0], lda, s->get_pointer(irrep), u->get_pointer(irrep)[0], ldu, vt->get_pointer(irrep)[0], ldvt, work->get_pointer(irrep), lwork, iwork->get_pointer(irrep));
}
/// PSI_DGESV, a wrapper to return C_DGESV using objects
int PSI_DGESV(int irrep, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb)
{
    return C_DGESV(n, nrhs, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DGESVX, a wrapper to return C_DGESVX using objects
int PSI_DGESVX(int irrep, char fact, char trans, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> af, int ldaf, shared_ptr<IntVector> ipiv, char equed, shared_ptr<Vector> r, shared_ptr<Vector> c, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> rcond, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGESVX(fact, trans, n, nrhs, a->get_pointer(irrep)[0], lda, af->get_pointer(irrep)[0], ldaf, ipiv->get_pointer(irrep), equed, r->get_pointer(irrep), c->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, rcond->get_pointer(irrep), ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGETRF, a wrapper to return C_DGETRF using objects
int PSI_DGETRF(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv)
{
    return C_DGETRF(m, n, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep));
}
/// PSI_DGETRI, a wrapper to return C_DGETRI using objects
int PSI_DGETRI(int irrep, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv, shared_ptr<Vector> work, int lwork)
{
    return C_DGETRI(n, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGETRS, a wrapper to return C_DGETRS using objects
int PSI_DGETRS(int irrep, char trans, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb)
{
    return C_DGETRS(trans, n, nrhs, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DGGBAK, a wrapper to return C_DGGBAK using objects
int PSI_DGGBAK(int irrep, char job, char side, int n, int ilo, int ihi, shared_ptr<Vector> lscale, shared_ptr<Vector> rscale, int m, shared_ptr<Matrix> v, int ldv)
{
    return C_DGGBAK(job, side, n, ilo, ihi, lscale->get_pointer(irrep), rscale->get_pointer(irrep), m, v->get_pointer(irrep)[0], ldv);
}
/// PSI_DGGBAL, a wrapper to return C_DGGBAL using objects
int PSI_DGGBAL(int irrep, char job, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<IntVector> ilo, shared_ptr<IntVector> ihi, shared_ptr<Vector> lscale, shared_ptr<Vector> rscale, shared_ptr<Vector> work)
{
    return C_DGGBAL(job, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, ilo->get_pointer(irrep), ihi->get_pointer(irrep), lscale->get_pointer(irrep), rscale->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DGGES, a wrapper to return C_DGGES using objects
int PSI_DGGES(int irrep, char jobvsl, char jobvsr, char sort, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<IntVector> sdim, shared_ptr<Vector> alphar, shared_ptr<Vector> alphai, shared_ptr<Vector> beta, shared_ptr<Matrix> vsl, int ldvsl, shared_ptr<Matrix> vsr, int ldvsr, shared_ptr<Vector> work, int lwork)
{
    return C_DGGES(jobvsl, jobvsr, sort, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, sdim->get_pointer(irrep), alphar->get_pointer(irrep), alphai->get_pointer(irrep), beta->get_pointer(irrep), vsl->get_pointer(irrep)[0], ldvsl, vsr->get_pointer(irrep)[0], ldvsr, work->get_pointer(irrep), lwork);
}
/// PSI_DGGESX, a wrapper to return C_DGGESX using objects
int PSI_DGGESX(int irrep, char jobvsl, char jobvsr, char sort, char sense, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<IntVector> sdim, shared_ptr<Vector> alphar, shared_ptr<Vector> alphai, shared_ptr<Vector> beta, shared_ptr<Matrix> vsl, int ldvsl, shared_ptr<Matrix> vsr, int ldvsr, shared_ptr<Vector> rconde, shared_ptr<Vector> rcondv, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DGGESX(jobvsl, jobvsr, sort, sense, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, sdim->get_pointer(irrep), alphar->get_pointer(irrep), alphai->get_pointer(irrep), beta->get_pointer(irrep), vsl->get_pointer(irrep)[0], ldvsl, vsr->get_pointer(irrep)[0], ldvsr, rconde->get_pointer(irrep), rcondv->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DGGEV, a wrapper to return C_DGGEV using objects
int PSI_DGGEV(int irrep, char jobvl, char jobvr, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> alphar, shared_ptr<Vector> alphai, shared_ptr<Vector> beta, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, shared_ptr<Vector> work, int lwork)
{
    return C_DGGEV(jobvl, jobvr, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, alphar->get_pointer(irrep), alphai->get_pointer(irrep), beta->get_pointer(irrep), vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, work->get_pointer(irrep), lwork);
}
/// PSI_DGGEVX, a wrapper to return C_DGGEVX using objects
int PSI_DGGEVX(int irrep, char balanc, char jobvl, char jobvr, char sense, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> alphar, shared_ptr<Vector> alphai, shared_ptr<Vector> beta, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, shared_ptr<IntVector> ilo, shared_ptr<IntVector> ihi, shared_ptr<Vector> lscale, shared_ptr<Vector> rscale, shared_ptr<Vector> abnrm, shared_ptr<Vector> bbnrm, shared_ptr<Vector> rconde, shared_ptr<Vector> rcondv, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork)
{
    return C_DGGEVX(balanc, jobvl, jobvr, sense, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, alphar->get_pointer(irrep), alphai->get_pointer(irrep), beta->get_pointer(irrep), vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, ilo->get_pointer(irrep), ihi->get_pointer(irrep), lscale->get_pointer(irrep), rscale->get_pointer(irrep), abnrm->get_pointer(irrep), bbnrm->get_pointer(irrep), rconde->get_pointer(irrep), rcondv->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep));
}
/// PSI_DGGGLM, a wrapper to return C_DGGGLM using objects
int PSI_DGGGLM(int irrep, int n, int m, int p, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> d, shared_ptr<Vector> x, shared_ptr<Vector> y, shared_ptr<Vector> work, int lwork)
{
    return C_DGGGLM(n, m, p, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, d->get_pointer(irrep), x->get_pointer(irrep), y->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGGHRD, a wrapper to return C_DGGHRD using objects
int PSI_DGGHRD(int irrep, char compq, char compz, int n, int ilo, int ihi, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> q, int ldq, shared_ptr<Matrix> z, int ldz)
{
    return C_DGGHRD(compq, compz, n, ilo, ihi, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, q->get_pointer(irrep)[0], ldq, z->get_pointer(irrep)[0], ldz);
}
/// PSI_DGGLSE, a wrapper to return C_DGGLSE using objects
int PSI_DGGLSE(int irrep, int m, int n, int p, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> c, shared_ptr<Vector> d, shared_ptr<Vector> x, shared_ptr<Vector> work, int lwork)
{
    return C_DGGLSE(m, n, p, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, c->get_pointer(irrep), d->get_pointer(irrep), x->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGGQRF, a wrapper to return C_DGGQRF using objects
int PSI_DGGQRF(int irrep, int n, int m, int p, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> taua, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> taub, shared_ptr<Vector> work, int lwork)
{
    return C_DGGQRF(n, m, p, a->get_pointer(irrep)[0], lda, taua->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, taub->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGGRQF, a wrapper to return C_DGGRQF using objects
int PSI_DGGRQF(int irrep, int m, int p, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> taua, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> taub, shared_ptr<Vector> work, int lwork)
{
    return C_DGGRQF(m, p, n, a->get_pointer(irrep)[0], lda, taua->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, taub->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DGGSVD, a wrapper to return C_DGGSVD using objects
int PSI_DGGSVD(int irrep, char jobu, char jobv, char jobq, int m, int n, int p, shared_ptr<IntVector> k, shared_ptr<IntVector> l, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> alpha, shared_ptr<Vector> beta, shared_ptr<Matrix> u, int ldu, shared_ptr<Matrix> v, int ldv, shared_ptr<Matrix> q, int ldq, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGGSVD(jobu, jobv, jobq, m, n, p, k->get_pointer(irrep), l->get_pointer(irrep), a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, alpha->get_pointer(irrep), beta->get_pointer(irrep), u->get_pointer(irrep)[0], ldu, v->get_pointer(irrep)[0], ldv, q->get_pointer(irrep)[0], ldq, work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGGSVP, a wrapper to return C_DGGSVP using objects
int PSI_DGGSVP(int irrep, char jobu, char jobv, char jobq, int m, int p, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, double tola, double tolb, shared_ptr<IntVector> k, shared_ptr<IntVector> l, shared_ptr<Matrix> u, int ldu, shared_ptr<Matrix> v, int ldv, shared_ptr<Matrix> q, int ldq, shared_ptr<IntVector> iwork, shared_ptr<Vector> tau, shared_ptr<Vector> work)
{
    return C_DGGSVP(jobu, jobv, jobq, m, p, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, tola, tolb, k->get_pointer(irrep), l->get_pointer(irrep), u->get_pointer(irrep)[0], ldu, v->get_pointer(irrep)[0], ldv, q->get_pointer(irrep)[0], ldq, iwork->get_pointer(irrep), tau->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DGTCON, a wrapper to return C_DGTCON using objects
int PSI_DGTCON(int irrep, char norm, int n, shared_ptr<Vector> dl, shared_ptr<Vector> d, shared_ptr<Vector> du, shared_ptr<Vector> du2, shared_ptr<IntVector> ipiv, double anorm, shared_ptr<Vector> rcond, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGTCON(norm, n, dl->get_pointer(irrep), d->get_pointer(irrep), du->get_pointer(irrep), du2->get_pointer(irrep), ipiv->get_pointer(irrep), anorm, rcond->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGTRFS, a wrapper to return C_DGTRFS using objects
int PSI_DGTRFS(int irrep, char trans, int n, int nrhs, shared_ptr<Vector> dl, shared_ptr<Vector> d, shared_ptr<Vector> du, shared_ptr<Vector> dlf, shared_ptr<Vector> df, shared_ptr<Vector> duf, shared_ptr<Vector> du2, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DGTRFS(trans, n, nrhs, dl->get_pointer(irrep), d->get_pointer(irrep), du->get_pointer(irrep), dlf->get_pointer(irrep), df->get_pointer(irrep), duf->get_pointer(irrep), du2->get_pointer(irrep), ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DGTSV, a wrapper to return C_DGTSV using objects
int PSI_DGTSV(int irrep, int n, int nrhs, shared_ptr<Vector> dl, shared_ptr<Vector> d, shared_ptr<Vector> du, shared_ptr<Matrix> b, int ldb)
{
    return C_DGTSV(n, nrhs, dl->get_pointer(irrep), d->get_pointer(irrep), du->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DGTSVX, a wrapper to return C_DGTSVX using objects
int PSI_DGTSVX(int irrep, char fact, char trans, int n, int nrhs, shared_ptr<Vector> dl, shared_ptr<Vector> d, shared_ptr<Vector> du, shared_ptr<Vector> dlf, shared_ptr<Vector> df, shared_ptr<Vector> duf, shared_ptr<Vector> du2, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> rcond)
{
    return C_DGTSVX(fact, trans, n, nrhs, dl->get_pointer(irrep), d->get_pointer(irrep), du->get_pointer(irrep), dlf->get_pointer(irrep), df->get_pointer(irrep), duf->get_pointer(irrep), du2->get_pointer(irrep), ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, rcond->get_pointer(irrep));
}
/// PSI_DGTTRF, a wrapper to return C_DGTTRF using objects
int PSI_DGTTRF(int irrep, int n, shared_ptr<Vector> dl, shared_ptr<Vector> d, shared_ptr<Vector> du, shared_ptr<Vector> du2, shared_ptr<IntVector> ipiv)
{
    return C_DGTTRF(n, dl->get_pointer(irrep), d->get_pointer(irrep), du->get_pointer(irrep), du2->get_pointer(irrep), ipiv->get_pointer(irrep));
}
/// PSI_DGTTRS, a wrapper to return C_DGTTRS using objects
int PSI_DGTTRS(int irrep, char trans, int n, int nrhs, shared_ptr<Vector> dl, shared_ptr<Vector> d, shared_ptr<Vector> du, shared_ptr<Vector> du2, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb)
{
    return C_DGTTRS(trans, n, nrhs, dl->get_pointer(irrep), d->get_pointer(irrep), du->get_pointer(irrep), du2->get_pointer(irrep), ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DHGEQZ, a wrapper to return C_DHGEQZ using objects
int PSI_DHGEQZ(int irrep, char job, char compq, char compz, int n, int ilo, int ihi, shared_ptr<Matrix> h, int ldh, shared_ptr<Matrix> t, int ldt, shared_ptr<Vector> alphar, shared_ptr<Vector> alphai, shared_ptr<Vector> beta, shared_ptr<Matrix> q, int ldq, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, int lwork)
{
    return C_DHGEQZ(job, compq, compz, n, ilo, ihi, h->get_pointer(irrep)[0], ldh, t->get_pointer(irrep)[0], ldt, alphar->get_pointer(irrep), alphai->get_pointer(irrep), beta->get_pointer(irrep), q->get_pointer(irrep)[0], ldq, z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), lwork);
}
/// PSI_DHSEIN, a wrapper to return C_DHSEIN using objects
int PSI_DHSEIN(int irrep, char side, char eigsrc, char initv, int n, shared_ptr<Matrix> h, int ldh, shared_ptr<Vector> wr, shared_ptr<Vector> wi, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, int mm, shared_ptr<IntVector> m, shared_ptr<Vector> work, shared_ptr<IntVector> ifaill, shared_ptr<IntVector> ifailr)
{
    return C_DHSEIN(side, eigsrc, initv, n, h->get_pointer(irrep)[0], ldh, wr->get_pointer(irrep), wi->get_pointer(irrep), vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, mm, m->get_pointer(irrep), work->get_pointer(irrep), ifaill->get_pointer(irrep), ifailr->get_pointer(irrep));
}
/// PSI_DHSEQR, a wrapper to return C_DHSEQR using objects
int PSI_DHSEQR(int irrep, char job, char compz, int n, int ilo, int ihi, shared_ptr<Matrix> h, int ldh, shared_ptr<Vector> wr, shared_ptr<Vector> wi, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, int lwork)
{
    return C_DHSEQR(job, compz, n, ilo, ihi, h->get_pointer(irrep)[0], ldh, wr->get_pointer(irrep), wi->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), lwork);
}
/// PSI_DORGBR, a wrapper to return C_DORGBR using objects
int PSI_DORGBR(int irrep, char vect, int m, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DORGBR(vect, m, n, k, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DORGHR, a wrapper to return C_DORGHR using objects
int PSI_DORGHR(int irrep, int n, int ilo, int ihi, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DORGHR(n, ilo, ihi, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DORGLQ, a wrapper to return C_DORGLQ using objects
int PSI_DORGLQ(int irrep, int m, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DORGLQ(m, n, k, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DORGQL, a wrapper to return C_DORGQL using objects
int PSI_DORGQL(int irrep, int m, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DORGQL(m, n, k, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DORGQR, a wrapper to return C_DORGQR using objects
int PSI_DORGQR(int irrep, int m, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DORGQR(m, n, k, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DORGRQ, a wrapper to return C_DORGRQ using objects
int PSI_DORGRQ(int irrep, int m, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DORGRQ(m, n, k, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DORGTR, a wrapper to return C_DORGTR using objects
int PSI_DORGTR(int irrep, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DORGTR(uplo, n, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DORMBR, a wrapper to return C_DORMBR using objects
int PSI_DORMBR(int irrep, char vect, char side, char trans, int m, int n, int k, shared_ptr<Vector> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work, int lwork)
{
    return C_DORMBR(vect, side, trans, m, n, k, a->get_pointer(irrep), lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep), lwork);
}
/// PSI_DORMHR, a wrapper to return C_DORMHR using objects
int PSI_DORMHR(int irrep, char side, char trans, int m, int n, int ilo, int ihi, shared_ptr<Vector> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work, int lwork)
{
    return C_DORMHR(side, trans, m, n, ilo, ihi, a->get_pointer(irrep), lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep), lwork);
}
/// PSI_DORMLQ, a wrapper to return C_DORMLQ using objects
int PSI_DORMLQ(int irrep, char side, char trans, int m, int n, int k, shared_ptr<Vector> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work, int lwork)
{
    return C_DORMLQ(side, trans, m, n, k, a->get_pointer(irrep), lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep), lwork);
}
/// PSI_DORMQL, a wrapper to return C_DORMQL using objects
int PSI_DORMQL(int irrep, char side, char trans, int m, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work, int lwork)
{
    return C_DORMQL(side, trans, m, n, k, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep), lwork);
}
/// PSI_DORMQR, a wrapper to return C_DORMQR using objects
int PSI_DORMQR(int irrep, char side, char trans, int m, int n, int k, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work, int lwork)
{
    return C_DORMQR(side, trans, m, n, k, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep), lwork);
}
/// PSI_DORMR3, a wrapper to return C_DORMR3 using objects
int PSI_DORMR3(int irrep, char side, char trans, int m, int n, int k, int l, shared_ptr<Vector> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work)
{
    return C_DORMR3(side, trans, m, n, k, l, a->get_pointer(irrep), lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep));
}
/// PSI_DORMRQ, a wrapper to return C_DORMRQ using objects
int PSI_DORMRQ(int irrep, char side, char trans, int m, int n, int k, shared_ptr<Vector> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work, int lwork)
{
    return C_DORMRQ(side, trans, m, n, k, a->get_pointer(irrep), lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep), lwork);
}
/// PSI_DORMRZ, a wrapper to return C_DORMRZ using objects
int PSI_DORMRZ(int irrep, char side, char trans, int m, int n, int k, int l, shared_ptr<Vector> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work, int lwork)
{
    return C_DORMRZ(side, trans, m, n, k, l, a->get_pointer(irrep), lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep), lwork);
}
/// PSI_DORMTR, a wrapper to return C_DORMTR using objects
int PSI_DORMTR(int irrep, char side, char uplo, char trans, int m, int n, shared_ptr<Vector> a, int lda, shared_ptr<Vector> tau, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> work, int lwork)
{
    return C_DORMTR(side, uplo, trans, m, n, a->get_pointer(irrep), lda, tau->get_pointer(irrep), c->get_pointer(irrep)[0], ldc, work->get_pointer(irrep), lwork);
}
/// PSI_DPBCON, a wrapper to return C_DPBCON using objects
int PSI_DPBCON(int irrep, char uplo, int n, int kd, shared_ptr<Matrix> ab, int ldab, double anorm, shared_ptr<Vector> rcond, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DPBCON(uplo, n, kd, ab->get_pointer(irrep)[0], ldab, anorm, rcond->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DPBEQU, a wrapper to return C_DPBEQU using objects
int PSI_DPBEQU(int irrep, char uplo, int n, int kd, shared_ptr<Matrix> ab, int ldab, shared_ptr<Vector> s, shared_ptr<Vector> scond, shared_ptr<Vector> amax)
{
    return C_DPBEQU(uplo, n, kd, ab->get_pointer(irrep)[0], ldab, s->get_pointer(irrep), scond->get_pointer(irrep), amax->get_pointer(irrep));
}
/// PSI_DPBRFS, a wrapper to return C_DPBRFS using objects
int PSI_DPBRFS(int irrep, char uplo, int n, int kd, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> afb, int ldafb, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DPBRFS(uplo, n, kd, nrhs, ab->get_pointer(irrep)[0], ldab, afb->get_pointer(irrep)[0], ldafb, b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DPBSTF, a wrapper to return C_DPBSTF using objects
int PSI_DPBSTF(int irrep, char uplo, int n, int kd, shared_ptr<Matrix> ab, int ldab)
{
    return C_DPBSTF(uplo, n, kd, ab->get_pointer(irrep)[0], ldab);
}
/// PSI_DPBSV, a wrapper to return C_DPBSV using objects
int PSI_DPBSV(int irrep, char uplo, int n, int kd, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> b, int ldb)
{
    return C_DPBSV(uplo, n, kd, nrhs, ab->get_pointer(irrep)[0], ldab, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DPBSVX, a wrapper to return C_DPBSVX using objects
int PSI_DPBSVX(int irrep, char fact, char uplo, int n, int kd, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> afb, int ldafb, char equed, shared_ptr<Vector> s, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> rcond, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DPBSVX(fact, uplo, n, kd, nrhs, ab->get_pointer(irrep)[0], ldab, afb->get_pointer(irrep)[0], ldafb, equed, s->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, rcond->get_pointer(irrep), ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DPBTRF, a wrapper to return C_DPBTRF using objects
int PSI_DPBTRF(int irrep, char uplo, int n, int kd, shared_ptr<Matrix> ab, int ldab)
{
    return C_DPBTRF(uplo, n, kd, ab->get_pointer(irrep)[0], ldab);
}
/// PSI_DPBTRS, a wrapper to return C_DPBTRS using objects
int PSI_DPBTRS(int irrep, char uplo, int n, int kd, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> b, int ldb)
{
    return C_DPBTRS(uplo, n, kd, nrhs, ab->get_pointer(irrep)[0], ldab, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DPOCON, a wrapper to return C_DPOCON using objects
int PSI_DPOCON(int irrep, char uplo, int n, shared_ptr<Matrix> a, int lda, double anorm, shared_ptr<Vector> rcond, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DPOCON(uplo, n, a->get_pointer(irrep)[0], lda, anorm, rcond->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DPOEQU, a wrapper to return C_DPOEQU using objects
int PSI_DPOEQU(int irrep, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> s, shared_ptr<Vector> scond, shared_ptr<Vector> amax)
{
    return C_DPOEQU(n, a->get_pointer(irrep)[0], lda, s->get_pointer(irrep), scond->get_pointer(irrep), amax->get_pointer(irrep));
}
/// PSI_DPORFS, a wrapper to return C_DPORFS using objects
int PSI_DPORFS(int irrep, char uplo, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> af, int ldaf, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DPORFS(uplo, n, nrhs, a->get_pointer(irrep)[0], lda, af->get_pointer(irrep)[0], ldaf, b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DPOSV, a wrapper to return C_DPOSV using objects
int PSI_DPOSV(int irrep, char uplo, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb)
{
    return C_DPOSV(uplo, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DPOSVX, a wrapper to return C_DPOSVX using objects
int PSI_DPOSVX(int irrep, char fact, char uplo, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> af, int ldaf, char equed, shared_ptr<Vector> s, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> rcond, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DPOSVX(fact, uplo, n, nrhs, a->get_pointer(irrep)[0], lda, af->get_pointer(irrep)[0], ldaf, equed, s->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, rcond->get_pointer(irrep), ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DPOTRF, a wrapper to return C_DPOTRF using objects
int PSI_DPOTRF(int irrep, char uplo, int n, shared_ptr<Matrix> a, int lda)
{
    return C_DPOTRF(uplo, n, a->get_pointer(irrep)[0], lda);
}
/// PSI_DPOTRI, a wrapper to return C_DPOTRI using objects
int PSI_DPOTRI(int irrep, char uplo, int n, shared_ptr<Matrix> a, int lda)
{
    return C_DPOTRI(uplo, n, a->get_pointer(irrep)[0], lda);
}
/// PSI_DPOTRS, a wrapper to return C_DPOTRS using objects
int PSI_DPOTRS(int irrep, char uplo, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb)
{
    return C_DPOTRS(uplo, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DPTCON, a wrapper to return C_DPTCON using objects
int PSI_DPTCON(int irrep, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, double anorm, shared_ptr<Vector> rcond, shared_ptr<Vector> work)
{
    return C_DPTCON(n, d->get_pointer(irrep), e->get_pointer(irrep), anorm, rcond->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DPTEQR, a wrapper to return C_DPTEQR using objects
int PSI_DPTEQR(int irrep, char compz, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work)
{
    return C_DPTEQR(compz, n, d->get_pointer(irrep), e->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep));
}
/// PSI_DPTRFS, a wrapper to return C_DPTRFS using objects
int PSI_DPTRFS(int irrep, int n, int nrhs, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Vector> df, shared_ptr<Vector> ef, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work)
{
    return C_DPTRFS(n, nrhs, d->get_pointer(irrep), e->get_pointer(irrep), df->get_pointer(irrep), ef->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DPTSV, a wrapper to return C_DPTSV using objects
int PSI_DPTSV(int irrep, int n, int nrhs, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> b, int ldb)
{
    return C_DPTSV(n, nrhs, d->get_pointer(irrep), e->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DPTSVX, a wrapper to return C_DPTSVX using objects
int PSI_DPTSVX(int irrep, char fact, int n, int nrhs, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Vector> df, shared_ptr<Vector> ef, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> rcond, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work)
{
    return C_DPTSVX(fact, n, nrhs, d->get_pointer(irrep), e->get_pointer(irrep), df->get_pointer(irrep), ef->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, rcond->get_pointer(irrep), ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DPTTRF, a wrapper to return C_DPTTRF using objects
int PSI_DPTTRF(int irrep, int n, shared_ptr<Vector> d, shared_ptr<Vector> e)
{
    return C_DPTTRF(n, d->get_pointer(irrep), e->get_pointer(irrep));
}
/// PSI_DPTTRS, a wrapper to return C_DPTTRS using objects
int PSI_DPTTRS(int irrep, int n, int nrhs, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> b, int ldb)
{
    return C_DPTTRS(n, nrhs, d->get_pointer(irrep), e->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DSBEV, a wrapper to return C_DSBEV using objects
int PSI_DSBEV(int irrep, char jobz, char uplo, int n, int kd, shared_ptr<Matrix> ab, int ldab, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work)
{
    return C_DSBEV(jobz, uplo, n, kd, ab->get_pointer(irrep)[0], ldab, w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep));
}
/// PSI_DSBEVD, a wrapper to return C_DSBEVD using objects
int PSI_DSBEVD(int irrep, char jobz, char uplo, int n, int kd, shared_ptr<Matrix> ab, int ldab, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSBEVD(jobz, uplo, n, kd, ab->get_pointer(irrep)[0], ldab, w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSBEVX, a wrapper to return C_DSBEVX using objects
int PSI_DSBEVX(int irrep, char jobz, char range, char uplo, int n, int kd, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> q, int ldq, double vl, double vu, int il, int iu, double abstol, shared_ptr<IntVector> m, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, shared_ptr<IntVector> iwork, shared_ptr<IntVector> ifail)
{
    return C_DSBEVX(jobz, range, uplo, n, kd, ab->get_pointer(irrep)[0], ldab, q->get_pointer(irrep)[0], ldq, vl, vu, il, iu, abstol, m->get_pointer(irrep), w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), iwork->get_pointer(irrep), ifail->get_pointer(irrep));
}
/// PSI_DSBGST, a wrapper to return C_DSBGST using objects
int PSI_DSBGST(int irrep, char vect, char uplo, int n, int ka, int kb, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> bb, int ldbb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> work)
{
    return C_DSBGST(vect, uplo, n, ka, kb, ab->get_pointer(irrep)[0], ldab, bb->get_pointer(irrep)[0], ldbb, x->get_pointer(irrep)[0], ldx, work->get_pointer(irrep));
}
/// PSI_DSBGV, a wrapper to return C_DSBGV using objects
int PSI_DSBGV(int irrep, char jobz, char uplo, int n, int ka, int kb, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> bb, int ldbb, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work)
{
    return C_DSBGV(jobz, uplo, n, ka, kb, ab->get_pointer(irrep)[0], ldab, bb->get_pointer(irrep)[0], ldbb, w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep));
}
/// PSI_DSBGVD, a wrapper to return C_DSBGVD using objects
int PSI_DSBGVD(int irrep, char jobz, char uplo, int n, int ka, int kb, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> bb, int ldbb, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSBGVD(jobz, uplo, n, ka, kb, ab->get_pointer(irrep)[0], ldab, bb->get_pointer(irrep)[0], ldbb, w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSBGVX, a wrapper to return C_DSBGVX using objects
int PSI_DSBGVX(int irrep, char jobz, char range, char uplo, int n, int ka, int kb, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> bb, int ldbb, shared_ptr<Matrix> q, int ldq, double vl, double vu, int il, int iu, double abstol, shared_ptr<IntVector> m, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, shared_ptr<IntVector> iwork, shared_ptr<IntVector> ifail)
{
    return C_DSBGVX(jobz, range, uplo, n, ka, kb, ab->get_pointer(irrep)[0], ldab, bb->get_pointer(irrep)[0], ldbb, q->get_pointer(irrep)[0], ldq, vl, vu, il, iu, abstol, m->get_pointer(irrep), w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), iwork->get_pointer(irrep), ifail->get_pointer(irrep));
}
/// PSI_DSBTRD, a wrapper to return C_DSBTRD using objects
int PSI_DSBTRD(int irrep, char vect, char uplo, int n, int kd, shared_ptr<Matrix> ab, int ldab, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> q, int ldq, shared_ptr<Vector> work)
{
    return C_DSBTRD(vect, uplo, n, kd, ab->get_pointer(irrep)[0], ldab, d->get_pointer(irrep), e->get_pointer(irrep), q->get_pointer(irrep)[0], ldq, work->get_pointer(irrep));
}
/// PSI_DSGESV, a wrapper to return C_DSGESV using objects
int PSI_DSGESV(int irrep, int n, int nrhs, shared_ptr<Vector> a, int lda, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Matrix> work, shared_ptr<IntVector> iter)
{
    return C_DSGESV(n, nrhs, a->get_pointer(irrep), lda, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, work->get_pointer(irrep)[0], iter->get_pointer(irrep));
}
/// PSI_DSTEBZ, a wrapper to return C_DSTEBZ using objects
int PSI_DSTEBZ(int irrep, char range, char order, int n, double vl, double vu, int il, int iu, double abstol, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<IntVector> m, shared_ptr<IntVector> nsplit, shared_ptr<Vector> w, shared_ptr<IntVector> iblock, shared_ptr<IntVector> isplit, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DSTEBZ(range, order, n, vl, vu, il, iu, abstol, d->get_pointer(irrep), e->get_pointer(irrep), m->get_pointer(irrep), nsplit->get_pointer(irrep), w->get_pointer(irrep), iblock->get_pointer(irrep), isplit->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DSTEDC, a wrapper to return C_DSTEDC using objects
int PSI_DSTEDC(int irrep, char compz, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSTEDC(compz, n, d->get_pointer(irrep), e->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSTEGR, a wrapper to return C_DSTEGR using objects
int PSI_DSTEGR(int irrep, char jobz, char range, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, double vl, double vu, int il, int iu, double abstol, shared_ptr<IntVector> m, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<IntVector> isuppz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSTEGR(jobz, range, n, d->get_pointer(irrep), e->get_pointer(irrep), vl, vu, il, iu, abstol, m->get_pointer(irrep), w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, isuppz->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSTEIN, a wrapper to return C_DSTEIN using objects
int PSI_DSTEIN(int irrep, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, int m, shared_ptr<Vector> w, shared_ptr<IntVector> iblock, shared_ptr<IntVector> isplit, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, shared_ptr<IntVector> iwork, shared_ptr<IntVector> ifail)
{
    return C_DSTEIN(n, d->get_pointer(irrep), e->get_pointer(irrep), m, w->get_pointer(irrep), iblock->get_pointer(irrep), isplit->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), iwork->get_pointer(irrep), ifail->get_pointer(irrep));
}
/// PSI_DSTEQR, a wrapper to return C_DSTEQR using objects
int PSI_DSTEQR(int irrep, char compz, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work)
{
    return C_DSTEQR(compz, n, d->get_pointer(irrep), e->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep));
}
/// PSI_DSTERF, a wrapper to return C_DSTERF using objects
int PSI_DSTERF(int irrep, int n, shared_ptr<Vector> d, shared_ptr<Vector> e)
{
    return C_DSTERF(n, d->get_pointer(irrep), e->get_pointer(irrep));
}
/// PSI_DSTEV, a wrapper to return C_DSTEV using objects
int PSI_DSTEV(int irrep, char jobz, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work)
{
    return C_DSTEV(jobz, n, d->get_pointer(irrep), e->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep));
}
/// PSI_DSTEVD, a wrapper to return C_DSTEVD using objects
int PSI_DSTEVD(int irrep, char jobz, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSTEVD(jobz, n, d->get_pointer(irrep), e->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSTEVR, a wrapper to return C_DSTEVR using objects
int PSI_DSTEVR(int irrep, char jobz, char range, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, double vl, double vu, int il, int iu, double abstol, shared_ptr<IntVector> m, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<IntVector> isuppz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSTEVR(jobz, range, n, d->get_pointer(irrep), e->get_pointer(irrep), vl, vu, il, iu, abstol, m->get_pointer(irrep), w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, isuppz->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSTEVX, a wrapper to return C_DSTEVX using objects
int PSI_DSTEVX(int irrep, char jobz, char range, int n, shared_ptr<Vector> d, shared_ptr<Vector> e, double vl, double vu, int il, int iu, double abstol, shared_ptr<IntVector> m, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, shared_ptr<IntVector> iwork, shared_ptr<IntVector> ifail)
{
    return C_DSTEVX(jobz, range, n, d->get_pointer(irrep), e->get_pointer(irrep), vl, vu, il, iu, abstol, m->get_pointer(irrep), w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), iwork->get_pointer(irrep), ifail->get_pointer(irrep));
}
/// PSI_DSYCON, a wrapper to return C_DSYCON using objects
int PSI_DSYCON(int irrep, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv, double anorm, shared_ptr<Vector> rcond, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DSYCON(uplo, n, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep), anorm, rcond->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DSYEV, a wrapper to return C_DSYEV using objects
int PSI_DSYEV(int irrep, char jobz, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> w, shared_ptr<Vector> work, int lwork)
{
    return C_DSYEV(jobz, uplo, n, a->get_pointer(irrep)[0], lda, w->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DSYEVD, a wrapper to return C_DSYEVD using objects
int PSI_DSYEVD(int irrep, char jobz, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> w, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSYEVD(jobz, uplo, n, a->get_pointer(irrep)[0], lda, w->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSYEVR, a wrapper to return C_DSYEVR using objects
int PSI_DSYEVR(int irrep, char jobz, char range, char uplo, int n, shared_ptr<Matrix> a, int lda, double vl, double vu, int il, int iu, double abstol, shared_ptr<IntVector> m, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<IntVector> isuppz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSYEVR(jobz, range, uplo, n, a->get_pointer(irrep)[0], lda, vl, vu, il, iu, abstol, m->get_pointer(irrep), w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, isuppz->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSYEVX, a wrapper to return C_DSYEVX using objects
int PSI_DSYEVX(int irrep, char jobz, char range, char uplo, int n, shared_ptr<Matrix> a, int lda, double vl, double vu, int il, int iu, double abstol, shared_ptr<IntVector> m, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, shared_ptr<IntVector> ifail)
{
    return C_DSYEVX(jobz, range, uplo, n, a->get_pointer(irrep)[0], lda, vl, vu, il, iu, abstol, m->get_pointer(irrep), w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), ifail->get_pointer(irrep));
}
/// PSI_DSYGST, a wrapper to return C_DSYGST using objects
int PSI_DSYGST(int irrep, int itype, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb)
{
    return C_DSYGST(itype, uplo, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DSYGV, a wrapper to return C_DSYGV using objects
int PSI_DSYGV(int irrep, int itype, char jobz, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> w, shared_ptr<Vector> work, int lwork)
{
    return C_DSYGV(itype, jobz, uplo, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, w->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DSYGVD, a wrapper to return C_DSYGVD using objects
int PSI_DSYGVD(int irrep, int itype, char jobz, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> w, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DSYGVD(itype, jobz, uplo, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, w->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DSYGVX, a wrapper to return C_DSYGVX using objects
int PSI_DSYGVX(int irrep, int itype, char jobz, char range, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, double vl, double vu, int il, int iu, double abstol, shared_ptr<IntVector> m, shared_ptr<Vector> w, shared_ptr<Matrix> z, int ldz, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, shared_ptr<IntVector> ifail)
{
    return C_DSYGVX(itype, jobz, range, uplo, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, vl, vu, il, iu, abstol, m->get_pointer(irrep), w->get_pointer(irrep), z->get_pointer(irrep)[0], ldz, work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), ifail->get_pointer(irrep));
}
/// PSI_DSYRFS, a wrapper to return C_DSYRFS using objects
int PSI_DSYRFS(int irrep, char uplo, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> af, int ldaf, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DSYRFS(uplo, n, nrhs, a->get_pointer(irrep)[0], lda, af->get_pointer(irrep)[0], ldaf, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DSYSV, a wrapper to return C_DSYSV using objects
int PSI_DSYSV(int irrep, char uplo, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> work, int lwork)
{
    return C_DSYSV(uplo, n, nrhs, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, work->get_pointer(irrep), lwork);
}
/// PSI_DSYSVX, a wrapper to return C_DSYSVX using objects
int PSI_DSYSVX(int irrep, char fact, char uplo, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> af, int ldaf, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> rcond)
{
    return C_DSYSVX(fact, uplo, n, nrhs, a->get_pointer(irrep)[0], lda, af->get_pointer(irrep)[0], ldaf, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, rcond->get_pointer(irrep));
}
/// PSI_DSYTRD, a wrapper to return C_DSYTRD using objects
int PSI_DSYTRD(int irrep, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> d, shared_ptr<Vector> e, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DSYTRD(uplo, n, a->get_pointer(irrep)[0], lda, d->get_pointer(irrep), e->get_pointer(irrep), tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DSYTRF, a wrapper to return C_DSYTRF using objects
int PSI_DSYTRF(int irrep, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv, shared_ptr<Vector> work, int lwork)
{
    return C_DSYTRF(uplo, n, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DSYTRI, a wrapper to return C_DSYTRI using objects
int PSI_DSYTRI(int irrep, char uplo, int n, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv, shared_ptr<Vector> work)
{
    return C_DSYTRI(uplo, n, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DSYTRS, a wrapper to return C_DSYTRS using objects
int PSI_DSYTRS(int irrep, char uplo, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<IntVector> ipiv, shared_ptr<Matrix> b, int ldb)
{
    return C_DSYTRS(uplo, n, nrhs, a->get_pointer(irrep)[0], lda, ipiv->get_pointer(irrep), b->get_pointer(irrep)[0], ldb);
}
/// PSI_DTBCON, a wrapper to return C_DTBCON using objects
int PSI_DTBCON(int irrep, char norm, char uplo, char diag, int n, int kd, shared_ptr<Matrix> ab, int ldab, shared_ptr<Vector> rcond, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DTBCON(norm, uplo, diag, n, kd, ab->get_pointer(irrep)[0], ldab, rcond->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DTBRFS, a wrapper to return C_DTBRFS using objects
int PSI_DTBRFS(int irrep, char uplo, char trans, char diag, int n, int kd, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DTBRFS(uplo, trans, diag, n, kd, nrhs, ab->get_pointer(irrep)[0], ldab, b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DTBTRS, a wrapper to return C_DTBTRS using objects
int PSI_DTBTRS(int irrep, char uplo, char trans, char diag, int n, int kd, int nrhs, shared_ptr<Matrix> ab, int ldab, shared_ptr<Matrix> b, int ldb)
{
    return C_DTBTRS(uplo, trans, diag, n, kd, nrhs, ab->get_pointer(irrep)[0], ldab, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DTGEVC, a wrapper to return C_DTGEVC using objects
int PSI_DTGEVC(int irrep, char side, char howmny, int n, shared_ptr<Matrix> s, int lds, shared_ptr<Matrix> p, int ldp, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, int mm, shared_ptr<IntVector> m, shared_ptr<Vector> work)
{
    return C_DTGEVC(side, howmny, n, s->get_pointer(irrep)[0], lds, p->get_pointer(irrep)[0], ldp, vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, mm, m->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DTGEXC, a wrapper to return C_DTGEXC using objects
int PSI_DTGEXC(int irrep, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> q, int ldq, shared_ptr<Matrix> z, int ldz, shared_ptr<IntVector> ifst, shared_ptr<IntVector> ilst, shared_ptr<Vector> work, int lwork)
{
    return C_DTGEXC(n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, q->get_pointer(irrep)[0], ldq, z->get_pointer(irrep)[0], ldz, ifst->get_pointer(irrep), ilst->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
/// PSI_DTGSEN, a wrapper to return C_DTGSEN using objects
int PSI_DTGSEN(int irrep, int ijob, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Vector> alphar, shared_ptr<Vector> alphai, shared_ptr<Vector> beta, shared_ptr<Matrix> q, int ldq, shared_ptr<Matrix> z, int ldz, shared_ptr<IntVector> m, shared_ptr<Vector> pl, shared_ptr<Vector> pr, shared_ptr<Vector> dif, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DTGSEN(ijob, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, alphar->get_pointer(irrep), alphai->get_pointer(irrep), beta->get_pointer(irrep), q->get_pointer(irrep)[0], ldq, z->get_pointer(irrep)[0], ldz, m->get_pointer(irrep), pl->get_pointer(irrep), pr->get_pointer(irrep), dif->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DTGSJA, a wrapper to return C_DTGSJA using objects
int PSI_DTGSJA(int irrep, char jobu, char jobv, char jobq, int m, int p, int n, int k, int l, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, double tola, double tolb, shared_ptr<Vector> alpha, shared_ptr<Vector> beta, shared_ptr<Matrix> u, int ldu, shared_ptr<Matrix> v, int ldv, shared_ptr<Matrix> q, int ldq, shared_ptr<Vector> work, shared_ptr<IntVector> ncycle)
{
    return C_DTGSJA(jobu, jobv, jobq, m, p, n, k, l, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, tola, tolb, alpha->get_pointer(irrep), beta->get_pointer(irrep), u->get_pointer(irrep)[0], ldu, v->get_pointer(irrep)[0], ldv, q->get_pointer(irrep)[0], ldq, work->get_pointer(irrep), ncycle->get_pointer(irrep));
}
/// PSI_DTGSNA, a wrapper to return C_DTGSNA using objects
int PSI_DTGSNA(int irrep, char job, char howmny, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, shared_ptr<Vector> s, shared_ptr<Vector> dif, int mm, shared_ptr<IntVector> m, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork)
{
    return C_DTGSNA(job, howmny, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, s->get_pointer(irrep), dif->get_pointer(irrep), mm, m->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep));
}
/// PSI_DTGSYL, a wrapper to return C_DTGSYL using objects
int PSI_DTGSYL(int irrep, char trans, int ijob, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> c, int ldc, shared_ptr<Matrix> d, int ldd, shared_ptr<Matrix> e, int lde, shared_ptr<Matrix> f, int ldf, shared_ptr<Vector> dif, shared_ptr<Vector> scale, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork)
{
    return C_DTGSYL(trans, ijob, m, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, c->get_pointer(irrep)[0], ldc, d->get_pointer(irrep)[0], ldd, e->get_pointer(irrep)[0], lde, f->get_pointer(irrep)[0], ldf, dif->get_pointer(irrep), scale->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep));
}
/// PSI_DTRCON, a wrapper to return C_DTRCON using objects
int PSI_DTRCON(int irrep, char norm, char uplo, char diag, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> rcond, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DTRCON(norm, uplo, diag, n, a->get_pointer(irrep)[0], lda, rcond->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DTREVC, a wrapper to return C_DTREVC using objects
int PSI_DTREVC(int irrep, char side, char howmny, int n, shared_ptr<Matrix> t, int ldt, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, int mm, shared_ptr<IntVector> m, shared_ptr<Vector> work)
{
    return C_DTREVC(side, howmny, n, t->get_pointer(irrep)[0], ldt, vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, mm, m->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DTREXC, a wrapper to return C_DTREXC using objects
int PSI_DTREXC(int irrep, char compq, int n, shared_ptr<Matrix> t, int ldt, shared_ptr<Matrix> q, int ldq, shared_ptr<IntVector> ifst, shared_ptr<IntVector> ilst, shared_ptr<Vector> work)
{
    return C_DTREXC(compq, n, t->get_pointer(irrep)[0], ldt, q->get_pointer(irrep)[0], ldq, ifst->get_pointer(irrep), ilst->get_pointer(irrep), work->get_pointer(irrep));
}
/// PSI_DTRRFS, a wrapper to return C_DTRRFS using objects
int PSI_DTRRFS(int irrep, char uplo, char trans, char diag, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> x, int ldx, shared_ptr<Vector> ferr, shared_ptr<Vector> berr, shared_ptr<Vector> work, shared_ptr<IntVector> iwork)
{
    return C_DTRRFS(uplo, trans, diag, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, x->get_pointer(irrep)[0], ldx, ferr->get_pointer(irrep), berr->get_pointer(irrep), work->get_pointer(irrep), iwork->get_pointer(irrep));
}
/// PSI_DTRSEN, a wrapper to return C_DTRSEN using objects
int PSI_DTRSEN(int irrep, char job, char compq, int n, shared_ptr<Matrix> t, int ldt, shared_ptr<Matrix> q, int ldq, shared_ptr<Vector> wr, shared_ptr<Vector> wi, shared_ptr<IntVector> m, shared_ptr<Vector> s, shared_ptr<Vector> sep, shared_ptr<Vector> work, int lwork, shared_ptr<IntVector> iwork, int liwork)
{
    return C_DTRSEN(job, compq, n, t->get_pointer(irrep)[0], ldt, q->get_pointer(irrep)[0], ldq, wr->get_pointer(irrep), wi->get_pointer(irrep), m->get_pointer(irrep), s->get_pointer(irrep), sep->get_pointer(irrep), work->get_pointer(irrep), lwork, iwork->get_pointer(irrep), liwork);
}
/// PSI_DTRSNA, a wrapper to return C_DTRSNA using objects
int PSI_DTRSNA(int irrep, char job, char howmny, int n, shared_ptr<Matrix> t, int ldt, shared_ptr<Matrix> vl, int ldvl, shared_ptr<Matrix> vr, int ldvr, shared_ptr<Vector> s, shared_ptr<Vector> sep, int mm, shared_ptr<IntVector> m, shared_ptr<Matrix> work, int ldwork, shared_ptr<IntVector> iwork)
{
    return C_DTRSNA(job, howmny, n, t->get_pointer(irrep)[0], ldt, vl->get_pointer(irrep)[0], ldvl, vr->get_pointer(irrep)[0], ldvr, s->get_pointer(irrep), sep->get_pointer(irrep), mm, m->get_pointer(irrep), work->get_pointer(irrep)[0], ldwork, iwork->get_pointer(irrep));
}
/// PSI_DTRSYL, a wrapper to return C_DTRSYL using objects
int PSI_DTRSYL(int irrep, char trana, char tranb, int isgn, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb, shared_ptr<Matrix> c, int ldc, shared_ptr<Vector> scale)
{
    return C_DTRSYL(trana, tranb, isgn, m, n, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb, c->get_pointer(irrep)[0], ldc, scale->get_pointer(irrep));
}
/// PSI_DTRTRI, a wrapper to return C_DTRTRI using objects
int PSI_DTRTRI(int irrep, char uplo, char diag, int n, shared_ptr<Matrix> a, int lda)
{
    return C_DTRTRI(uplo, diag, n, a->get_pointer(irrep)[0], lda);
}
/// PSI_DTRTRS, a wrapper to return C_DTRTRS using objects
int PSI_DTRTRS(int irrep, char uplo, char trans, char diag, int n, int nrhs, shared_ptr<Matrix> a, int lda, shared_ptr<Matrix> b, int ldb)
{
    return C_DTRTRS(uplo, trans, diag, n, nrhs, a->get_pointer(irrep)[0], lda, b->get_pointer(irrep)[0], ldb);
}
/// PSI_DTZRQF, a wrapper to return C_DTZRQF using objects
int PSI_DTZRQF(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau)
{
    return C_DTZRQF(m, n, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep));
}
/// PSI_DTZRZF, a wrapper to return C_DTZRZF using objects
int PSI_DTZRZF(int irrep, int m, int n, shared_ptr<Matrix> a, int lda, shared_ptr<Vector> tau, shared_ptr<Vector> work, int lwork)
{
    return C_DTZRZF(m, n, a->get_pointer(irrep)[0], lda, tau->get_pointer(irrep), work->get_pointer(irrep), lwork);
}
}

