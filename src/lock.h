#ifndef UUNIX_LOCK
#define UUNIX_LOCK

#include "base.h" // IWYU pragma: keep
#include "hart.h"
#include "trap.h"

#define UNLOCKED_NUM ((u64)0)
#define LOCKED_NUM   (~UNLOCKED_NUM)

#define spin2lock(lock)                                                   \
    do {                                                                  \
        while (strict_swap((lock), LOCKED_NUM) != UNLOCKED_NUM)           \
            ;                                                             \
    } while (0)

#define spin2unlock(lock)                                                 \
    do {                                                                  \
        strict_swap((lock), UNLOCKED_NUM);                                \
    } while (0)

#endif // UUNIX_LOCK
