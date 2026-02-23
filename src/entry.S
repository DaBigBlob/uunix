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
    /* our board has 5 harts -->  */
    .space 10 * 1024 * 1024 /* 10MiB */
stack_base:

.section .data
.globl ready
ready:
    .quad hart_wait
