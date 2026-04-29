#ifndef UUNIX_HART
#define UUNIX_HART
#include "pre.h"
#include "base.S.h"

#define MAX_HARTS       256
#define HART_STACK_SIZE ((STACK_SIZE) / (MAX_HARTS))

/**  Design
For each hart:
    - have a stack and a hart control block (HCB).
    - HCB specifies the task to be done.
    - HCB operactions are atomic.
    - once HCB task is done.
    - waits for task in HCB.
    - by default set to execute main.
In main:
    Hart non-0:
        - immediately complete task i.e. wait on HCB
    Hart 0:
        - setup stuff
        - set other's HCB
For each hart:
    - atomic spin on HCB.jump_addr till its non empty
    - when non-empty, load a5-a0 from stack and jump to *HCB.jump_addr
    - after job:
        - clear HCB.jump_addr
    - repeat
*/

/* 64 bytes, alignment 8 */
typedef struct {
    usize a0;
    usize a1;
    usize a2;
    usize a3;
    usize a4;
    usize a5;
    usize lock;
    addr  jump_addr;
} HCB;

#define check_offset(n, o)                                                \
    _Static_assert(offsetof(HCB, n) == o, "bad " #n " offset")
check_offset(a0, 0x00);
check_offset(a1, 0x08);
check_offset(a2, 0x10);
check_offset(a3, 0x18);
check_offset(a4, 0x20);
check_offset(a5, 0x28);
check_offset(lock, 0x30);
check_offset(jump_addr, 0x38);
#undef check_offset

#define M_get_HCB_addr(hartid)                                            \
    ((volatile addr)(kstack_base - (HART_STACK_SIZE * hartid) -           \
                     sizeof(HCB)))

addr get_HCB_addr(void);
void reset_HCB(void);

#define spin2unlock(lock) strict_swap((lock), 0)
#define spin2lock(lock)                                                   \
    do {                                                                  \
    } while (strict_swap((lock), 077777777) != 0)
#define new_spinlock() ((u64)(0))

noreturn void hart_done(void);

void hart_task(usize hartid, usize a0, usize a1, usize a2, usize a3,
               usize a4, usize a5, addr jump_addr);

/** Logistics
    We manage no more than 256 harts (ids: 0 to 255).
    256 * 4096 = 256 * 2^12 = 1,048,576 = 1MiB

    Of that 4096 = 4KiB stack, 64bytes are reserved for HCB.
*/

#endif // UUNIX_HART
