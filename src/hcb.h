#ifndef UUNIX_HCB
#define UUNIX_HCB

#define REGISTER_LIST(x, y)                                               \
    x(ra) x(sp) x(gp) x(tp) x(t0) x(t1) x(t2) x(s0) x(s1) x(a0) x(a1)     \
        x(a2) x(a3) x(a4) x(a5) x(a6) x(a7) x(s2) x(s3) x(s4) x(s5) x(s6) \
            x(s7) x(s8) x(s9) x(s10) x(s11) x(t3) x(t4) x(t5) x(t6)       \
                x(mepc) x(mstatus) x(mcause) y(mtval)

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

#ifndef ASSEMBLYTIME
#include "pre.h"

#define df0(a) a,
#define df1(a) a
/* hart-specific control block */
typedef struct {
    usize REGISTER_LIST(df0, df1);
    usize hartid;
} HCB;
#undef df0
#undef df1

#define cof(n, o)                                                         \
    _Static_assert(offsetof(HCB, n) == o, "bad " #n " offset")
#define df0(a) cof(a, HCB_OFFSET_##a);
REGISTER_LIST(df0, df0)
#undef df0
#undef cof

#define compute_HCB_addr(hartid)                                          \
    ((volatile addr)(kstack_base - (HART_STACK_SIZE * (hartid + 1)) -     \
                     sizeof(HCB)))
#else // ASSEMBLYTIME

#endif // ASSEMBLYTIME

#endif // UUNIX_HCB
