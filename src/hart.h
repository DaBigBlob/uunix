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

/* hart-specific control block */
typedef struct {
    usize a0;
    usize a1;
    usize a2;
    usize a3;
    usize a4;
    usize a5;
    usize lock;
    addr  jump_addr;
    struct { /* things a hart might want to store */
        usize mstatus;
    } store;
} HCB;

#define M_get_HCB_addr(hartid)                                            \
    ((volatile addr)(kstack_base - (HART_STACK_SIZE * hartid) -           \
                     sizeof(HCB)))

addr get_HCB_addr(void);

noreturn void hart_done(void);

void hart_task(usize hartid, usize a0, usize a1, usize a2, usize a3,
               usize a4, usize a5, addr jump_addr);

/** Logistics
    We manage no more than 256 harts (ids: 0 to 255).
    256 * 4096 = 256 * 2^12 = 1,048,576 = 1MiB

    Of that 4096 = 4KiB stack, 64bytes are reserved for HCB.
*/

#endif // UUNIX_HART
