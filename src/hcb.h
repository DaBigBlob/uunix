#ifndef UUNIX_HCB
#define UUNIX_HCB

#define REGISTER_LIST_a(x, y)                                             \
    x(a0) x(a1) x(a2) x(a3) x(a4) x(a5) x(a6) y(a7)
/* t5 and t6 will be used for mscratch fun in trap entry */
#define REGISTER_LIST_NOm_NOt5t6(x, y)                                    \
    x(ra) x(sp) x(gp) x(tp) x(t0) x(t1) x(t2) x(s0) x(s1)                 \
        REGISTER_LIST_a(x, x) x(s2) x(s3) x(s4) x(s5) x(s6) x(s7) x(s8)   \
            x(s9) x(s10) x(s11) x(t3) y(t4)
#define REGISTER_LIST_NOm(x, y) REGISTER_LIST_NOm_NOt5t6(x, x) x(t5) y(t6)
#define REGISTER_LIST_m_NOint(x, y) x(mepc) y(mstatus)
#define REGISTER_LIST_m(x, y)                                             \
    REGISTER_LIST_m_NOint(x, x) x(mcause) y(mtval)
#define REGISTER_LIST(x, y) REGISTER_LIST_NOm(x, x) REGISTER_LIST_m(x, y)

#define HCB_OFFSET_ra      0x000
#define HCB_OFFSET_sp      0x008
#define HCB_OFFSET_gp      0x010
#define HCB_OFFSET_tp      0x018
#define HCB_OFFSET_t0      0x020
#define HCB_OFFSET_t1      0x028
#define HCB_OFFSET_t2      0x030
#define HCB_OFFSET_s0      0x038
#define HCB_OFFSET_s1      0x040
#define HCB_OFFSET_a0      0x048
#define HCB_OFFSET_a1      0x050
#define HCB_OFFSET_a2      0x058
#define HCB_OFFSET_a3      0x060
#define HCB_OFFSET_a4      0x068
#define HCB_OFFSET_a5      0x070
#define HCB_OFFSET_a6      0x078
#define HCB_OFFSET_a7      0x080
#define HCB_OFFSET_s2      0x088
#define HCB_OFFSET_s3      0x090
#define HCB_OFFSET_s4      0x098
#define HCB_OFFSET_s5      0x0a0
#define HCB_OFFSET_s6      0x0a8
#define HCB_OFFSET_s7      0x0b0
#define HCB_OFFSET_s8      0x0b8
#define HCB_OFFSET_s9      0x0c0
#define HCB_OFFSET_s10     0x0c8
#define HCB_OFFSET_s11     0x0d0
#define HCB_OFFSET_t3      0x0d8
#define HCB_OFFSET_t4      0x0e0
#define HCB_OFFSET_t5      0x0e8
#define HCB_OFFSET_t6      0x0f0
#define HCB_OFFSET_mepc    0x0f8
#define HCB_OFFSET_mstatus 0x100
#define HCB_OFFSET_mcause  0x108
#define HCB_OFFSET_mtval   0x110

#ifndef ASSEMBLYTIME // C begin ---------------------

#include "pre.h"

/* hart-specific control block */
typedef struct {
    /* for trap frame */
#define df0(a) any a;
    REGISTER_LIST(df0, df0)
#undef df0
    usize hartid;

    /* for M-mode hart-tasks */
    struct {
        struct {
#define df0(a) any a;
            REGISTER_LIST_a(df0, df0)
#undef df0
        } args;
        any sp;
        any func;
    } cmd;
} HCB;

/* check: the offsets are valid */
#define cof(n, o)                                                         \
    _Static_assert(offsetof(HCB, n) == o, "bad " #n " offset")
#define df0(a) cof(a, HCB_OFFSET_##a);
REGISTER_LIST(df0, df0)
/* no need to check lock because no asm access */
#undef df0
#undef cof

#endif // C end -------------------------------------

#endif // UUNIX_HCB
