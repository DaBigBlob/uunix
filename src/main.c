#include "base.h"
#include "pre.h"
#include "std.h"
#include "uart.h"
#include "hcb.h"
#include "hart.h"
#include "trap.h"

noreturn void main(void)
{
    usize hart = get_mhartid();

    set_mstatus(get_mstatus() & ~MASK_MSTATUS_MIE);

    if (hart == 0) {
        mem_set(bss_begin, bss_end, volatile addr, 0);
        uart_init(uart0);
        uart_init(uart1);
    }

    /* per-hart setup */
    set_mscratch((any)compute_HCB_addr(hart));
    mscratch2HCB()->hartid = hart;

    set_mtvec((any)trap_entry);

    unset_msip(hart);

    set_mie((get_mie() & ~(MASK_MIE_MTIE | MASK_MIE_MEIE)) |
            MASK_MIE_MSIE);

    set_mstatus(get_mstatus() | MASK_MSTATUS_MIE);

    if (hart != 0) {
        for (;;)
            wait4int();
    }

    uart_puts(uart0, "hi\r\n");

    set_msip(1); /* interrupt hart 1 */

    for (;;)
        wait4int();
}
