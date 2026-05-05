#ifndef UUNIX_BASE
#define UUNIX_BASE

// One must bargain with the machine.

#define STACK_SIZE 1048576 /* 1MiB */

#ifndef ASSEMBLYTIME
#include "pre.h"
#endif // ASSEMBLYTIME

#ifndef ASSEMBLYTIME
#define UNIDEF(ret, name, args) extern ret name args
#else  // ASSEMBLYTIME
// clang-format off

#define UNIDEF(ret, name, args) .globl name;

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
    ret;                    \
ENDF(get_##thing)

// clang-format on
#endif // ASSEMBLYTIME

UNIDEF(usize, get_mhartid, (void));

UNIDEF(usize, get_mstatus, (void));
UNIDEF(void, set_mstatus, (usize));

UNIDEF(usize, get_mie, (void));
UNIDEF(void, set_mie, (usize));

UNIDEF(void, set_mtvec, (usize));

UNIDEF(usize, get_mcause, (void));

UNIDEF(u64, strict_swap, (volatile u64 * at, u64 with));

UNIDEF(noreturn void, hart_begin,
       (usize a0, usize a1, usize a2, usize a3, usize a4, usize a5,
        usize sp, addr jump_addr));

UNIDEF(volatile addr, bss_begin, []);
UNIDEF(volatile addr, bss_end, []);
UNIDEF(const addr, kstack_base, []);
UNIDEF(const addr, kheap_top, []);

#endif // UUNIX_BASE
