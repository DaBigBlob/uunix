#ifndef UUNIX_LOCK
#define UUNIX_LOCK

#include "pre.h"

#define UNLOCKED_NUM ((u64)0)
#define LOCKED_NUM   (~UNLOCKED_NUM)

typedef struct {
    u64   lock;
    usize mstatus;
} spinlock_t;

#define init_spinlock(slock)                                              \
    do {                                                                  \
        (slock).lock    = UNLOCKED_NUM;                                   \
        (slock).mstatus = 0;                                              \
    } while (0)

void spin2lock(spinlock_t *lock);
void spin2unlock(spinlock_t *lock);

typedef u64 dumblock_t;

#define init_dumblock(slock)                                              \
    do {                                                                  \
        slock = UNLOCKED_NUM;                                             \
    } while (0)

#define dumb2lock(slock)                                                  \
    do {                                                                  \
        while (strict_swap(&slock, LOCKED_NUM) != UNLOCKED_NUM)           \
            ;                                                             \
    } while (0)

#define dumb2unlock(slock)                                                \
    do {                                                                  \
        strict_swap(&slock, UNLOCKED_NUM);                                \
    } while (0)

#endif // UUNIX_LOCK
