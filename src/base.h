#ifndef UUNIX_BASE
#define UUNIX_BASE

// What must exist at the bottom of the world?

#define STACK_SIZE 1048576 /* 1MiB */

#ifndef ASSEMBLYTIME
#include "pre.h"
#endif // ASSEMBLYTIME

#ifndef ASSEMBLYTIME
#define CASMD(ret, name, args) extern ret name args
#else  // ASSEMBLYTIME
// clang-format off

#define CASMD(ret, name, args) .globl name;

#define FUNC(name)      \
.section .text;         \
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

CASMD(usize, get_mhartid, (void));
CASMD(usize, get_mstatus, (void));
CASMD(void, set_mstatus, (usize));
CASMD(u64, strict_swap, (volatile u64 * at, u64 with));
CASMD(noreturn void, hart_begin,
      (usize a0, usize a1, usize a2, usize a3, usize a4, usize a5,
       usize sp, addr jump_addr));

CASMD(volatile addr, bss_begin, []);
CASMD(volatile addr, bss_end, []);
CASMD(const addr, kstack_base, []);
CASMD(const addr, kheap_top, []);

#endif // UUNIX_BASE
