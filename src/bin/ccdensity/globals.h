/*! \file
    \ingroup CCDENSITY
    \brief Enter brief description of file here 
*/
#include <ccfiles.h>
#include <libdpd/dpd.h>
#include <psi4-dec.h>

namespace psi { namespace CCDENSITY {

/* Global variables */
#ifdef EXTERN
#undef EXTERN
#define EXTERN extern
#else
#define EXTERN
#endif

/* #define DEBUG_XI (1)*/

EXTERN struct MOInfo moinfo;
EXTERN struct Frozen frozen;
EXTERN struct Params params;
EXTERN struct RHO_Params *rho_params;
EXTERN struct TD_Params *td_params;

}} // namespace psi::ccdensity
