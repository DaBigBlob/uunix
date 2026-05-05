#define ASSEMBLYTIME
#include "base.h"
.extern main
.extern impl_get_HCB_addr

/*--- begin: stack alloc ---*/
.section .bss
.globl kheap_top
.globl kstack_base
kheap_top:
    .space STACK_SIZE
kstack_base:
/*--- end: stack alloc ---*/


FUNC(uunix_entry)
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
ENDF(uunix_entry)


GETTER(mhartid)
GETTER(mstatus)
SETTER(mstatus)
GETTER(mie)
SETTER(mie)
SETTER(mtvec)
GETTER(mcause)

FUNC(strict_swap)
/* trying to be a little defensive */
    fence   iorw, iorw
    amoswap.d.aqrl  t0, a1, 0(a0)
    fence   iorw, iorw
    mv      a0, t0
    ret
ENDF(strict_swap)

FUNC(hart_begin)
    mv  sp, a6
    jr  a7
ENDF(hart_begin)
