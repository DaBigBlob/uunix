#ifndef UUNIX_HART
#define UUNIX_HART
#include "pre.h"
#include "base.S.h"

#define MAX_HARTS       256
#define HART_STACK_SIZE ((STACK_SIZE) / (MAX_HARTS))

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
    struct { /* things a hart might want to temporarily store */
        usize mstatus;
    } store;
} HCB;

#define get_HCB_addr(hartid)                                              \
    ((volatile addr)(kstack_base - (HART_STACK_SIZE * hartid) -           \
                     sizeof(HCB)))

addr impl_get_HCB_addr(void);

noreturn void hart_done(void);

void hart_task(usize hartid, usize a0, usize a1, usize a2, usize a3,
               usize a4, usize a5, addr jump_addr);

/** Logistics
    We manage no more than 256 harts (ids: 0 to 255).
    256 * 4096 = 256 * 2^12 = 1,048,576 = 1MiB

    Of that 4096 = 4KiB stack, 64bytes are reserved for HCB.
*/

#endif // UUNIX_HART
