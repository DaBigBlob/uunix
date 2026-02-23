.extern main

.section .text
.globl _start
_start:
    csrr t0, mhartid
    la   sp, stack_base
    slli t1, t0, 12 /* 2^12 = 4096 bytes for each hart */
    sub  sp, sp, t1
    /* send non0 harts to waiting */
    bnez t0, hart_wait

    /* for hart0 */
    call main             /* may return */
    j dead_spin

hart_wait:
    la  t0, ready
    ld  t0, 0(t0)
    jr  t0

dead_spin:
    j   dead_spin

.section .bss
    .space 128 * 1024 /* 100KiB */
stack_base:

.section .data
.globl ready
ready:
    .quad hart_wait
