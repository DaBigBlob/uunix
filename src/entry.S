.extern main

.section .bss
heap_top:
    .space 1024 * 1024 /* 1MiB */
stack_base:

.section .text
.globl _start
.globl dead_spin
_start:
    csrr a0, mhartid
    la   sp, stack_base
    slli t1, a0, 12 /* 2^12 = 4096 bytes = 4KiB for each hart */
    sub  sp, sp, t1
    /* send non0 harts to waiting */
    bnez a0, hart_wait

    /* for hart0 */
    j   main
    /* fall through if returned */

dead_spin:
    j   dead_spin




.section .data
.globl ready
ready:
    .quad hart_wait

.section .text
hart_wait:
    la  t0, ready
    ld  t0, 0(t0)
    jr  t0
