#ifndef UUNIX_BASE
#define UUNIX_BASE
#include "pre.h"

// What must exist at the bottom of the world?

extern volatile addr bss_begin[], bss_end[];
extern volatile addr kheap_top[], kstack_base[];

extern noreturn void dead_spin(void);
extern u64           get_hartid(void);

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
    - sp naturally sits 64bits above assigned hart stack base
    - atomic spin on HCB.jump_addr till its non empty
    - when non-empty, load a6-a0 from stack and jump to *HCB.jump_addr
    - reusing the space for initial a6-a0 on stack
    - after job:
        - clear HCB.jump_addr
    - repeat
*/

/* 64 bytes, alignment 8 */
typedef struct {
    u64   a0;
    u64   a1;
    u64   a2;
    u64   a3;
    u64   a4;
    u64   a5;
    u64   a6;
    void *jump_addr;
} HCB;

#define check_offset(n, o)                                                \
    _Static_assert(offsetof(HCB, n) == o, "bad " #n " offset")
check_offset(a0, 0x00);
check_offset(a1, 0x08);
check_offset(a2, 0x10);
check_offset(a3, 0x18);
check_offset(a4, 0x20);
check_offset(a5, 0x28);
check_offset(a6, 0x30);
check_offset(jump_addr, 0x38);
#undef check_offset

/** spin_lockhart
    - compare and swap:
        - compare with: 0 i.e. unlocked
        - swap with self.hartid
*/

/** hart_done
    - clear HCB.jump_addr
    - set sp 64bytes above assigned hart stack base
    - atomic spin on HCB.jump_addr till its non empty
    - when non-empty:
        - load a6-a0 from HCB and jump to *HCB.jump_addr
    - after job:
        - repeat
*/
extern noreturn void hart_done(void);
extern void hart_task(u64 a0, u64 a1, u64 a2, u64 a3, u64 a4, u64 a5,
                      u64 a6, void *jump_addr); /* atomically set HCB */

/** Logistics
    We manage no more than 256 harts (ids: 0 to 255).
    256 * 4096 = 256 * 2^12 = 1,048,576 = 1MiB

    Of that 4096 = 4KiB stack, 64bytes are reserved for HCB.
*/

#endif // UUNIX_BASE
