#ifndef UUNIX_LOCK
#define UUNIX_LOCK

#include "base.h"
#include "hart.h"
#include "interr.h"

#define spin2lock(lock)                                                   \
    do {                                                                  \
        usize _uunis_priv__spin2unlock_hartid = get_hartid();             \
        MIE_storeNunset(_uunis_priv__spin2unlock_hartid);                 \
        while (strict_swap((lock), 077777777) != 0)                       \
            ;                                                             \
    } while (0)

#define spin2unlock(lock)                                                 \
    do {                                                                  \
        strict_swap((lock), 0);                                           \
        usize _uunis_priv__spin2unlock_hartid = get_hartid();             \
        restore_mstatus(_uunis_priv__spin2unlock_hartid);                 \
    } while (0)

#define new_spinlock() ((u64)(0))

#endif // UUNIX_LOCK
