#ifndef UUNIX_BASE
#define UUNIX_BASE

// What must exist at the bottom of the world?

#define STACK_SIZE 1048576 /* 1MiB */

#ifndef ASSEMBLYTIME
#include "pre.h"

extern volatile addr bss_begin[], bss_end[];
extern const addr    kheap_top[], kstack_base[];

extern usize         get_mhartid(void);
extern usize         get_mstatus(void);
extern void          set_mstatus(usize);
extern u64           strict_swap(volatile u64 *at, u64 with);
noreturn extern void hart_begin(usize a0, usize a1, usize a2, usize a3,
                                usize a4, usize a5, usize sp,
                                addr jump_addr);

#else  // ASSEMBLYTIME
// clang-format off

#define FUNC(name)      \
.section .text;         \
.globl name;            \
.type name, @function;  \
name:

#define ENDF(name) .size name, .-name

#define GETTER(thing)   \
FUNC(get_##thing);      \
    csrr    a0, thing;  \
    ret;                \
ENDF(get_##thing)

#define SETTER(thing)       \
FUNC(set_##thing);          \
    csrw    mstatus, a0;    \
    ret;                \
ENDF(get_##thing)

// clang-format on
#endif // ASSEMBLYTIME
#endif // UUNIX_BASE
