#define ASSEMBLYTIME
#include "base.h"

/* pre c-call begin: unsafe to call c functions ***************************/
.section .bss
    .space 524288 /* 2^19 bytes */
_uunix_entry_tmp_stack:

.section .text._uunix_entry
_uunix_entry:
    la      sp, _uunix_entry_tmp_stack
    csrr    a0, mhartid
    slli    t1, a0, 11 /* 2^11 = 2048 bytes = 2KiB for each hart */
    sub     sp, sp, t1
/* pre c-call end *********************************************************/

    /* set correct sp */
    call    ASMUSE_get_hstackbase /* must use less than 2KiB */
    mv      sp, a0

    csrr    a0, mhartid /* debug point + main arg */

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

FUNC(wait4int)
    wfi
    ret
ENDF(wait4int)

FUNC(trap_entry)
    /* store register */
    csrrw   t6, mscratch, t6        /* ms(HB) <-> t6 */
    sd      t5, HCB_OFFSET_t5(t6)   /* t5 -> HB^ */
    csrr    t5, mscratch            /* t5 <- ms(t6) */
    sd      t5, HCB_OFFSET_t6(t6)   /* t5(t6) -> HB^ */
    csrw  mscratch, t6              /* t6(HB) -> ms*/

    /* the rest of the non-m ones */
#define df0(a) sd a, HCB_OFFSET_##a(t6);
    REGISTER_LIST_NOm_NOt5t6(df0, df0)
#undef df0

    /* the m ones */
#define df0(a) csrr t5, a; sd t5, HCB_OFFSET_##a(t6);
    REGISTER_LIST_m(df0, df0)
#undef df0
    /* all regesters saved: we are free to call stuff */

    /* set safe sp */
    call    ASMUSE_get_hstackbase
    mv      sp, a0

    /* the actual meat */
    call   trap_handle

    /* restore register: easier this time */
    csrr t6, mscratch

    /* the m ones (except int specific ones) */
#define df0(a) ld t5, HCB_OFFSET_##a(t6); csrw a, t5;
    REGISTER_LIST_m_NOint(df0, df0)
#undef df0

    /* the non-m ones */
#define df0(a) ld a, HCB_OFFSET_##a(t6);
    REGISTER_LIST_NOm(df0, df0)
#undef df0
    /* ^notice: t6 is restored last; and its safe (i think) */

    mret
ENDF(trap_entry)
