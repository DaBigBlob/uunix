#ifndef UUNIX_HCB
#define UUNIX_HCB

#define REGISTER_LIST_a(x, s)                                             \
    x(a0) s x(a1)                                                         \
    s       x(a2)                                                         \
    s       x(a3)                                                         \
    s       x(a4)                                                         \
    s       x(a5)                                                         \
    s       x(a6)                                                         \
    s       x(a7)
/* t5 and t6 will be used for mscratch fun in trap entry */
#define REGISTER_LIST_NOm_NOt5t6(x, s)                                    \
    x(ra) s x(sp)                                                         \
    s       x(gp)                                                         \
    s       x(tp)                                                         \
    s       x(t0)                                                         \
    s       x(t1)                                                         \
    s       x(t2)                                                         \
    s       x(s0)                                                         \
    s       x(s1)                                                         \
    s       REGISTER_LIST_a(x, s)                                         \
    s       x(s2)                                                         \
    s       x(s3)                                                         \
    s       x(s4)                                                         \
    s       x(s5)                                                         \
    s       x(s6)                                                         \
    s       x(s7)                                                         \
    s       x(s8)                                                         \
    s       x(s9)                                                         \
    s       x(s10)                                                        \
    s       x(s11)                                                        \
    s       x(t3)                                                         \
    s       x(t4)
#define REGISTER_LIST_NOm(x, s)                                           \
    REGISTER_LIST_NOm_NOt5t6(x, s) s x(t5)                                \
    s                                x(t6)
#define REGISTER_LIST_m_NOint(x, s) x(mepc) s x(mstatus)
#define REGISTER_LIST_m(x, s)                                             \
    REGISTER_LIST_m_NOint(x, s) s x(mcause)                               \
    s                             x(mtval)
#define REGISTER_LIST(x, s) REGISTER_LIST_NOm(x, s) s REGISTER_LIST_m(x, s)

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
#include "lock.h"

/* hart-specific control block */
typedef struct {
    /* for trap frame */
#define df0(a) any a
    REGISTER_LIST(df0, ;);
#undef df0
    usize hartid;

    /* for M-mode hart-tasks */
    struct {
        struct {
#define df0(a) any a
            REGISTER_LIST_a(df0, ;);
#undef df0
        } args;
        any        sp;
        any        ra;
        any        func;
        dumblock_t lock;
    } cmd;
} HCB;

/* check: the offsets are valid */
#define cof(n, o)                                                         \
    _Static_assert(offsetof(HCB, n) == o, "bad " #n " offset")
#define df0(a) cof(a, HCB_OFFSET_##a)
REGISTER_LIST(df0, ;);
/* no need to check lock because no asm access */
#undef df0
#undef cof

#endif // C end -------------------------------------

#endif // UUNIX_HCB
