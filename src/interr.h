#ifndef UUNIX_INTERR
#define UUNIX_INTERR

#include "base.h"

#define MSTATUS_MIE ((usize)1u << 3)

#define mstatus_store() mstatus_get()

#endif // UUNIX_INTERR
