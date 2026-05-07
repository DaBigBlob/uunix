#include "lock.h"
#include "base.h"
#include "trap.h"

void spin2lock(spinlock_t *lock)
{
    const usize mstatus = get_mstatus();
    set_mstatus(mstatus & ~MASK_MSTATUS_MIE); // disable int

    /* try to acquire lock: if not get back UNLOCKED_NUM,
    someone beat us so keep spinning */
    while (strict_swap(&lock->lock, LOCKED_NUM) != UNLOCKED_NUM)
        ;

    lock->mstatus = mstatus; // save if acquired
}

void spin2unlock(spinlock_t *lock)
{
    strict_swap(&lock->lock, UNLOCKED_NUM);
    set_mstatus(lock->mstatus);
}
