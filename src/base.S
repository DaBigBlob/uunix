#define ASSEMBLYTIME
#include "base.h"

/* pre func begin: unsafe to call functions (no safe stack) **************/
.section .data
.balign 8
_tmp_jump_addr:
    .dword _tmp_wait4bss0

.section .bss
.balign 16 /* riscv abi need */
    .space 524288 /* (2^19 = 524288) / (2^11 = 2048) = 256 max harts */
_tmp_stack:

FUNC(_uunix_entry)
    csrci   mstatus, 8 /* no int till mtvec/mscratch/HCB init */

    /* send non0 harts to wait */
    csrr    a0, mhartid
    bnez    a0, _tmp_wait4bss0

    /* clear bss */
    la      t0, bss_begin
    la      t1, bss_end
_tmp_clear_bss:
    bgeu    t0, t1, _tmp_clear_bss_done
    sb      zero, 0(t0)
    addi    t0, t0, 1
    j       _tmp_clear_bss
_tmp_clear_bss_done:
    fence   rw, w /* make bss0 visible before setting jump_addr */
    la      t0, _tmp_jump_addr
    la      t1, _tmp_stack_init_done
    sd      t1, 0(t0)
    j       _tmp_stack_init_done


_tmp_wait4bss0:
    la      t0, _tmp_jump_addr
    ld      t1, 0(t0)
    fence   r, rw   /* be safe: prevent moving mem rw to before here */
    jr      t1


_tmp_stack_init_done:
    la      sp, _tmp_stack
    csrr    a0, mhartid
    slli    t1, a0, 11 /* 2^11 = 2048 bytes = 2KiB for each hart */
    sub     sp, sp, t1
/* pre func end **********************************************************/

    /* set correct sp */
    call    ASMUSE_get_hstackbase /* must use less than 2KiB */
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

FUNC(wait4int)
    wfi
    ret
ENDF(wait4int)

FUNC(fence_mem)
    fence rw, rw
    ret
ENDF(fence_mem)

FUNC(trap_entry)
    /* store register */
    csrrw   t6, mscratch, t6        /* ms(HB) <-> t6    */
    sd      t5, HCB_OFFSET_t5(t6)   /* t5 -> HB^        */
    csrr    t5, mscratch            /* t5 <- ms(t6)     */
    sd      t5, HCB_OFFSET_t6(t6)   /* t5(t6) -> HB^    */
    csrw    mscratch, t6            /* t6(HB) -> ms     */

    /* the rest of the non-m ones */
#define df0(a) sd a, HCB_OFFSET_##a(t6)
    REGISTER_LIST_NOm_NOt5t6(df0, ;);
#undef df0

    /* the m ones */
#define df0(a) csrr t5, a; sd t5, HCB_OFFSET_##a(t6)
    REGISTER_LIST_m(df0, ;);
#undef df0
    /* all regesters saved: we are free to call stuff */

    /* set needed sp */
    csrr    t5, mscratch /* has hstackbase */
    mv      sp, t5

    /* the actual meat */
    call    trap_handle

    /* restore register: easier this time */
    csrr    t6, mscratch

    /* the m ones (except int specific ones) */
#define df0(a) ld t5, HCB_OFFSET_##a(t6); csrw a, t5
    REGISTER_LIST_m_NOint(df0, ;);
#undef df0

    /* the non-m ones */
#define df0(a) ld a, HCB_OFFSET_##a(t6)
    REGISTER_LIST_NOm(df0, ;);
#undef df0
    /* ^notice: t6 is restored last; and its safe (i think) */

    mret
ENDF(trap_entry)

FUNC(spin_ecall)
    ecall
1:
    j 1b
ENDF(spin_ecall)
