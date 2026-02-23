.extern main

.section .bss
    .space 128 * 1024 /* 100KiB */
stack_base:

.section .text
.globl _start
.globl dead_spin
_start:
    csrr a0, mhartid
    la   sp, stack_base
    slli t1, a0, 12 /* 2^12 = 4096 bytes for each hart */
    sub  sp, sp, t1
    /* send non0 harts to waiting */
    bnez a0, hart_wait

    /* for hart0 */
    j main             /* may return */
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
