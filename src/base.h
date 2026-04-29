#ifndef UUNIX_BASE
#define UUNIX_BASE
#include "pre.h"

// What must exist at the bottom of the world?

/********* Receive */
extern volatile addr bss_begin[], bss_end[];
extern const addr    kheap_top[], kstack_base[];

extern noreturn void dead_spin(void);
extern usize         get_hartid(void);
extern u64           strict_swap(volatile u64 *at, u64 with);
noreturn extern void hart_begin(usize a0, usize a1, usize a2, usize a3,
                                usize a4, usize a5, usize sp,
                                addr jump_addr);

#define HART_STACK_SIZE 4096

#endif // UUNIX_BASE
