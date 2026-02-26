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

_Static_assert(offsetof(clck_t, hfxosccfg)         == 0x00, "bad txdata offset");
_Static_assert(offsetof(clck_t, corepllcfg0)       == 0x04, "bad rxdata offset");
_Static_assert(offsetof(clck_t, ddrpllcfg0)        == 0x0C, "bad txctrl offset");
_Static_assert(offsetof(clck_t, ddrpllcfg1)        == 0x10, "bad rxctrl offset");
_Static_assert(offsetof(clck_t, gemgxlpllcfg0)     == 0x1C, "bad ie offset");
_Static_assert(offsetof(clck_t, gemgxlpllcfg1)     == 0x20, "bad ip offset");
_Static_assert(offsetof(clck_t, coreclksel)        == 0x24, "bad div offset");
_Static_assert(offsetof(clck_t, devicesresetreg)   == 0x28, "bad div offset");

#endif // UUNIX_CLOCK
