#ifndef UUNIX_HCB
#define UUNIX_HCB
#include "pre.h"
#include "base.h"

#define MAX_HARTS       256
#define HART_STACK_SIZE ((STACK_SIZE) / (MAX_HARTS))

/* hart-specific control block */
typedef struct {
    struct {
        usize ra, sp, gp, tp;
        usize t0, t1, t2;
        usize s0, s1;
        usize a0, a1, a2, a3, a4, a5, a6, a7;
        usize s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
        usize t3, t4, t5, t6;
        usize mepc, mstatus, mcause, mtval;
    } reg;
    usize hartid;
} HCB;

#define compute_HCB_addr(hartid)                                          \
    ((volatile addr)(kstack_base - (HART_STACK_SIZE * (hartid + 1)) -     \
                     sizeof(HCB)))

#endif // UUNIX_HCB
