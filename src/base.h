#ifndef UUNIX_BASE
#define UUNIX_BASE

// One must bargain with the machine.

#ifndef ASSEMBLYTIME // C begin ---------------------

#include "pre.h"

#define UNIDEF(rett, name, args)     extern rett name args
#define UNIDEFGET(rett, name, thing) UNIDEF(rett, name, (void))
#define UNIDEFSET(thing, name, argt) UNIDEF(void, name, (argt))

#else  // C end / ASSEMBLYTIME begin ----------------
// clang-format off
#include "hcb.h" /* safe: hcb.h has ASSEMBLYTIME */

#define FUNC(name)      \
.section .text.##name;  \
.globl name;            \
.type name, @function;  \
.balign 4;              \
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
#endif // ASSEMBLYTIME end --------------------------
UNIDEFGET(usize, get_mhartid, mhartid);
UNIDEFGET(usize, get_mstatus, mstatus);
UNIDEFGET(usize, get_mie, mie);
UNIDEFGET(usize, get_mcause, mcause);
UNIDEFGET(usize, get_mepc, mepc);
UNIDEFGET(usize, get_mtval, mtval);

UNIDEFSET(mstatus, set_mstatus, usize);
UNIDEFSET(mie, set_mie, usize);
UNIDEFSET(mtvec, set_mtvec, any);
UNIDEFSET(mepc, set_mepc, usize);
UNIDEFSET(mscratch, set_mscratch_hstackbase, any);
UNIDEFSET(pmpaddr0, set_pmpaddr0, usize);
UNIDEFSET(pmpcfg0, set_pmpcfg0, usize);

UNIDEF(u64, strict_swap, (volatile u64 * at, u64 with));
UNIDEF(u64, wait4int, (void));
UNIDEF(void, trap_entry, (void));
UNIDEF(any, ASMUSE_get_hstackbase, (void));

#endif // UUNIX_BASE
