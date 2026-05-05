#ifndef UUNIX_LOCK
#define UUNIX_LOCK

#include "base.h"

#define spin2unlock(lock) strict_swap((lock), 0)
#define spin2lock(lock)                                                   \
    do {                                                                  \
    } while (strict_swap((lock), 077777777) != 0)
#define new_spinlock() ((u64)(0))

#endif // UUNIX_LOCK
