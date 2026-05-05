#include "base.S.h"
.extern main
.extern impl_get_HCB_addr

.section .bss
.globl kheap_top
.globl kstack_base
kheap_top:
    .space STACK_SIZE
kstack_base:

.section .text
.globl uunix_entry
uunix_entry:
    /* pre c-call begin:  unsafe to call c functions */
    la  sp, kstack_base
    csrr    a0, mhartid
    slli    t1, a0, 12 /* 2^12 = 4096 bytes = 4KiB for each hart */
    sub sp, sp, t1
        /* these hart-specific stack locations (and stack sizes)
        are temporary; permanent ones are decided in hart.c */
    /* pre c-call end */

    /* set final sp */
    call impl_get_HCB_addr
    mv  sp, a0

    j   main

/*--- begin: macros ---*/
#define FUNC(name)  \
.section .text;     \
.globl name;        \
name:

#define GETTER(thing)   \
FUNC(get_##thing);            \
    csrr    a0, thing;   \
    ret
/*--- end: macros ---*/

GETTER(mhartid)

.section .text
.globl get_mstatus
get_mstatus:
    csrr    a0, mstatus
    ret

.section .text
.globl set_mstatus
set_mstatus:
    csrw    mstatus, a0
    ret

.section .text
.globl strict_swap
strict_swap:
/* trying to be a little defensive */
    fence   iorw, iorw
    amoswap.d.aqrl  t0, a1, 0(a0)
    fence   iorw, iorw
    mv      a0, t0
    ret

.section .text
.globl hart_begin
hart_begin:
    mv  sp, a6
    jr  a7
