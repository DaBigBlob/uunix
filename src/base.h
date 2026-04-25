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

typedef struct {
    u64   a[7];
    void *jump_addr;
} HCB;

extern volatile addr bss_begin[], bss_end[];
extern volatile addr kheap_top[], kstack_base[];

extern noreturn void dead_spin(void);
extern u64           get_hartid(void);

#endif // UUNIX_BASE
