#ifndef UUNIX_INTERR
#define UUNIX_INTERR

#include "pre.h"
#include "hart.h"

extern volatile u32 clint[];

#define get_msip(hartid) (clint[hartid] & 1)
#define set_msip(hartid)                                                  \
    do {                                                                  \
        clint[hartid] = 1;                                                \
    } while (0)
#define unset_msip(hartid)                                                \
    do {                                                                  \
        clint[hartid] = 0;                                                \
    } while (0)

#define MSTATUS_MIE ((usize)1u << 3)

#define MIE_storeNunset(hartid)                                           \
    do {                                                                  \
        store_mstatus(hartid);                                            \
        set_mstatus(get_mstatus() & (~MSTATUS_MIE));                      \
    } while (0)

#endif // UUNIX_INTERR
