#ifndef UUNIX_INTERR
#define UUNIX_INTERR

#include "base.h"

#define MSTATUS_MIE ((usize)1u << 3)

extern u64 hart_temp_mstatus;

#endif // UUNIX_INTERR
