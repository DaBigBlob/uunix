#ifndef UUNIX_BASE
#define UUNIX_BASE

// One must bargain with the machine.

#define STACK_SIZE 1048576 /* 1MiB */

#ifndef ASSEMBLYTIME
#include "pre.h"
#endif // ASSEMBLYTIME

#ifndef ASSEMBLYTIME

#define UNIDEF(rett, name, args)     extern rett name args
#define UNIDEFGET(rett, name, thing) UNIDEF(rett, name, (void))
#define UNIDEFSET(thing, name, argt) UNIDEF(void, name, (argt))

#else  // ASSEMBLYTIME
// clang-format off

#define FUNC(name)      \
.section .text;         \
.type name, @function;  \
name:

#define ENDF(name) .size name, .-name

#define UNIDEF(rett, name, args) .globl name;
#define UNIDEFGET(rett, name, thing)\
UNIDEF(rett, name, (void));         \
FUNC(name);                         \
    csrr    a0, thing;              \
    ret;                            \
ENDF(name)

#define UNIDEFSET(thing, name, argt)\
UNIDEF(void, name, (argt))          \
FUNC(name);                         \
    csrw    thing, a0;              \
    ret;                            \
ENDF(name)

// clang-format on
#endif // ASSEMBLYTIME

UNIDEFGET(usize, get_mhartid, mhartid);
UNIDEFGET(usize, get_mstatus, mstatus);
UNIDEFGET(usize, get_mie, mie);
UNIDEFGET(usize, get_mcause, mcause);

UNIDEFSET(mstatus, set_mstatus, usize);
UNIDEFSET(mie, set_mie, usize);
UNIDEFSET(mtvec, set_mtvec, usize);

UNIDEF(u64, strict_swap, (volatile u64 * at, u64 with));

UNIDEF(noreturn void, hart_begin,
       (usize a0, usize a1, usize a2, usize a3, usize a4, usize a5,
        usize sp, addr jump_addr));

UNIDEF(volatile addr, bss_begin, []);
UNIDEF(volatile addr, bss_end, []);
UNIDEF(const addr, kstack_base, []);
UNIDEF(const addr, kheap_top, []);

#endif // UUNIX_BASE
