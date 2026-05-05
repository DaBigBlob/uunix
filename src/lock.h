#ifndef UUNIX_LOCK
#define UUNIX_LOCK

#include "base.h" // IWYU pragma: keep
#include "hart.h"
#include "interr.h"

#define UNLOCKED_NUM ((u64)0)
#define LOCKED_NUM   (~UNLOCKED_NUM)

#define spin2lock(lock)                                                   \
    do {                                                                  \
        usize _uunis_priv__spin2unlock_hartid = get_hartid();             \
        MIE_storeNunset(_uunis_priv__spin2unlock_hartid);                 \
        while (strict_swap((lock), LOCKED_NUM) != UNLOCKED_NUM)           \
            ;                                                             \
    } while (0)

#define spin2unlock(lock)                                                 \
    do {                                                                  \
        strict_swap((lock), UNLOCKED_NUM);                                \
        usize _uunis_priv__spin2unlock_hartid = get_hartid();             \
        restore_mstatus(_uunis_priv__spin2unlock_hartid);                 \
    } while (0)

#define new_spinlock() ((u64)(0))

#endif // UUNIX_LOCK
