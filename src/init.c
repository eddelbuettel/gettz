#include <R.h>
#include <Rinternals.h>
#include <stdlib.h> // for NULL
#include <R_ext/Rdynload.h>

/* FIXME: 
   Check these declarations against the C/Fortran source code.
*/

/* .Call calls */
extern SEXP gettz_impl(void);

static const R_CallMethodDef CallEntries[] = {
    {"gettz_impl", (DL_FUNC) &gettz_impl, 0},
    {NULL, NULL, 0}
};

void R_init_gettz(DllInfo *dll)
{
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
