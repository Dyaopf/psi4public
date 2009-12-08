/*! \file
    \ingroup CCTRIPLES
    \brief Enter brief description of file here 
*/
#include <cstdio>
#include <ccfiles.h>
#include <libdpd/dpd.h>
#include <psi4-dec.h>

namespace psi { namespace cctriples {

/* Global variables */
#ifdef EXTERN
#undef EXTERN
#define EXTERN extern
#else
#define EXTERN
#endif

EXTERN struct MOInfo moinfo;
EXTERN struct Params params;

}} // namespace psi::CCTRIPLES
