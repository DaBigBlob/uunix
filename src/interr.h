#ifndef UUNIX_INTERR
#define UUNIX_INTERR

#include "hart.h"

#define MSTATUS_MIE ((usize)1u << 3)

#define store_mstatus()                                                   \
    do {                                                                  \
        HCB_set_fld(get_hartid(), store).mstatus = get_mstatus();         \
    } while (0)

#define restore_mstatus()                                                 \
    set_mstatus(HCB_set_fld(get_hartid(), store).mstatus)

#endif // UUNIX_INTERR
