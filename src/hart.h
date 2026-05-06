#ifndef UUNIX_HART
#define UUNIX_HART
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

#define get_HCB_addr(hartid) compute_HCB_addr(hartid)

#define HCB_set_fld(hartid, fld) ((HCB *)get_HCB_addr(hartid))->fld

#define store_mstatus(hartid)                                             \
    do {                                                                  \
        HCB_set_fld(hartid, store).mstatus = get_mstatus();               \
    } while (0)

#define restore_mstatus(hartid)                                           \
    set_mstatus(HCB_set_fld(hartid, store).mstatus)

addr impl_get_HCB_addr(void);

#endif // UUNIX_HART
