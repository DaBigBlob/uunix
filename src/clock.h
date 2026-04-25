#ifndef UUNIX_CLOCK
#define UUNIX_CLOCK
#include "pre.h"

typedef struct {
    u32 hfxosccfg;
    u32 corepllcfg0;
    u32 gap0[1];
    u32 ddrpllcfg0;
    u32 ddrpllcfg1;
    u32 gap1[2];
    u32 gemgxlpllcfg0;
    u32 gemgxlpllcfg1;
    u32 coreclksel;
    u32 devicesresetreg;
} clck_t;

#define check_offset(o, n)                                                \
    _Static_assert(offsetof(clck_t, n) == o, "bad " #n " offset")

check_offset(0x00, hfxosccfg);
check_offset(0x04, corepllcfg0);
check_offset(0x0C, ddrpllcfg0);
check_offset(0x10, ddrpllcfg1);
check_offset(0x1C, gemgxlpllcfg0);
check_offset(0x20, gemgxlpllcfg1);
check_offset(0x24, coreclksel);
check_offset(0x28, devicesresetreg);

#undef check_offset

extern volatile clck_t clckctrl;

/*
"FU540-C000 generates all internal clocks from 33.33MHz hfclk driven
from an external oscillator (HFCLKIN) or crystal (HFOSCIN) input,
selected by inputHFXSEL" - SiFive manual
*/
#define HFCLK_HZ 33330000

#define CORECLK_DIVR() ((clckctrl.corepllcfg0 & BIT_MASK(6, 0, 32)) >> 0)
#define CORECLK_DIVF() ((clckctrl.corepllcfg0 & BIT_MASK(15, 6, 32)) >> 6)
#define CORECLK_DIVQ()                                                    \
    ((clckctrl.corepllcfg0 & BIT_MASK(18, 15, 32)) >> 15)

#define CORECLK_HZ()                                                      \
    (HFCLK_HZ * ((clckctrl.coreclksel & 0x1) ?                            \
                     1 :                                                  \
                     ((2 * (CORECLK_DIVF() + 1)) /                        \
                      ((CORECLK_DIVR() + 1) * (1U << CORECLK_DIVQ())))))

#define TLCLK_HZ() (CORECLK_HZ() / 2)

#endif // UUNIX_CLOCK
