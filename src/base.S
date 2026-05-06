#define ASSEMBLYTIME
#include "base.h"
#include "hcb.h"

/*--- begin: stack alloc ---*/
.section .bss
kheap_top:
    .space STACK_SIZE
kstack_base:
/*--- end: stack alloc ---*/

.section .text._uunix_entry
_uunix_entry:
    /* pre c-call begin:  unsafe to call c functions */
    la      sp, kstack_base
    csrr    a0, mhartid
    slli    t1, a0, 12 /* 2^12 = 4096 bytes = 4KiB for each hart */
    sub     sp, sp, t1
        /* these hart-specific stack locations (and stack sizes)
        are temporary; permanent ones are decided in hart.c */
    /* pre c-call end */

    /* set final sp */
    call    ASMUSE_get_HCB_addr
    mv      sp, a0

    j   main
ENDF(_uunix_entry)

FUNC(strict_swap)
/* trying to be a little defensive */
    fence   iorw, iorw
    amoswap.d.aqrl  t0, a1, 0(a0)
    fence   iorw, iorw
    mv      a0, t0
    ret
ENDF(strict_swap)

FUNC(hart_begin)
    mv  sp, a6
    jr  a7
ENDF(hart_begin)

FUNC(wait4int)
    wfi
    ret
ENDF(wait4int)

FUNC(trap_entry)
    csrrw   t6, mscratch, t6        /* ms(HB) <-> t6 */
    sd      t5, HCB_OFFSET_t5(t6)   /* t5 -> HB^ */
    csrr    t5, mscratch            /* t5 <- ms(t6) */
    sd      t5, HCB_OFFSET_t6(t6)   /* t5(t6) -> HB^ */

    /* the rest of the non-m ones */
#define df0(a) sd a, HCB_OFFSET_##a(t6);
    REGISTER_LIST_NOm_NOt5t6(df0, df0)
#undef df0

    /* the m ones */
#define df0(a) csrr t5, a; sd t5, HCB_OFFSET_##a(t6);
    REGISTER_LIST_m(df0, df0)
#undef df0

    /* all regesters saved: we are free to call stuff */

    /* restore mscratch */
    call    ASMUSE_get_HCB_addr
    csrw    mscratch, a0;

    j   trap_handle
ENDF(trap_entry)
