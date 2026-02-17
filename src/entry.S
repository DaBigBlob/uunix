.extern __stack_base
.extern __ready
.extern __main

.section .text
.globl __entry
__entry:
    /* set up spinlock */
    la t0, __ready
    la t1, hart_wait
    sd t1, 0(t0)

    csrr t0, mhartid
    la   sp, __stack_base
    slli t0, t0, 12
    sub  sp, sp, t0

    /* send non0 harts to waiting */
    csrr t0, mhartid
    bnez t0, hart_wait

    /* for hart0 */
    call __main             /* may return */
    j dead_spin

hart_wait:
    la  t0, __ready
    ld  t0, 0(t0)
    jr  t0

dead_spin:
    j   dead_spin
