#ifndef UUNIX_LOCK
#define UUNIX_LOCK

#include "base.h"
#include "interr.h"

#define spin2lock(lock)                                                   \
    do {                                                                  \
        store_mstatus();                                                  \
        while (strict_swap((lock), 077777777) != 0)                       \
            ;                                                             \
    } while (0)

#define spin2unlock(lock)                                                 \
    do {                                                                  \
        strict_swap((lock), 0);                                           \
        restore_mstatus();                                                \
    } while (0)

#define new_spinlock() ((u64)(0))

#endif // UUNIX_LOCK
