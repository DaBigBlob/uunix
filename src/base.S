#include "base.S.h"
.extern main
.extern get_HCB_addr

.section .bss
.globl kheap_top
.globl kstack_base
kheap_top:
    .space STACK_SIZE
kstack_base:

.section .text
.globl _start
_start:
    /* pre c-call begin:  unsafe to call c functions */
    la   sp, kstack_base
    csrr a0, mhartid
    slli t1, a0, 12 /* 2^12 = 4096 bytes = 4KiB for each hart */
    sub  sp, sp, t1
        /* these hart-specific stack locations are temporary;
        permanent ones are decided in base.c */
    /* pre c-call end */

    /* set final sp */
    call get_HCB_addr
    mv sp, a0

    j   main
    j   dead_spin

.section .text
.globl dead_spin
dead_spin:
    j   dead_spin


.section .text
.globl get_hartid
get_hartid:
    csrr a0, mhartid
    ret

.section .text
.globl strict_swap
strict_swap:
/* trying to be a little defensive */
    fence iorw, iorw
    amoswap.d.aqrl  t0, a1, 0(a0)
    fence iorw, iorw
    mv              a0, t0
    ret

.section .text
.globl hart_begin
hart_begin:
    mv  sp, a6
    jr  a7
