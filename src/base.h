#ifndef UUNIX_BASE
#define UUNIX_BASE
#include "pre.h"

// What must exist at the bottom of the world?

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
*/

/* 64 bytes, alignment 8 */
typedef struct {
    u64   a0;
    u64   a1;
    u64   a2;
    u64   a3;
    u64   a4;
    u64   a5;
    u64   hartid;
    void *jump_addr;
} HCB;

extern noreturn void hart_done(void); /* clear HCB & atomic spin on HCB */
extern void hart_task(u64 a0, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5,
                      u64   hartid,
                      void *jump_addr); /* atomically set HCB */

/** Logistics
    We manage no more than 256 harts.
    256 * 4096 = 256 * 2^12 = 1,048,576 = 1MiB

    Of that 4096 = 4KiB stack, 64bytes are reserved for HCB.
*/

extern volatile addr bss_begin[], bss_end[];
extern volatile addr kheap_top[], kstack_base[];

extern noreturn void dead_spin(void);
extern u64           get_hartid(void);

#endif // UUNIX_BASE
