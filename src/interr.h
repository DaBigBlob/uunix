#ifndef UUNIX_INTERR
#define UUNIX_INTERR

#include "hart.h"

#define MSTATUS_MIE ((usize)1u << 3)

#define MIE_storeNunset(hartid)                                           \
    do {                                                                  \
        store_mstatus(hartid);                                            \
        set_mstatus(get_mstatus() & (~MSTATUS_MIE));                      \
    } while (0)

#endif // UUNIX_INTERR
