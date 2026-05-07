#include "pre.h"
#include "base.h"
#include "trap.h"
#include "mem.h"
#include "uart.h"

noreturn void main(void)
{
    set_mstatus(get_mstatus() & ~MASK_MSTATUS_MIE); // disable int

    usize hartid = get_mhartid();

    if (hartid == 0) {
        uart_init(uart0);
        uart_init(uart1);
    }

    /* setup int */
    set_mtvec((any)trap_entry); // set int vec
    set_mie((get_mie() & ~(MASK_MIE_MTIE | MASK_MIE_MEIE)) |
            MASK_MIE_MSIE); // enable soft int only
    unset_msip(hartid);     // clear self soft int

    /* setup HCB */
    set_mscratch_hstackbase(compute_hartid2hstackbase(hartid));
    compute_hstackbase2HCB(get_mscratch_hstackbase())->hartid = hartid;

    // int still disabled
    /*********************************************************************/

    if (hartid == 0) {
        // command
    }

    for (;;)
        wait4int(); // wait for task
}
