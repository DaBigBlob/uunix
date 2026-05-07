#include "lock.h"
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
        init_lock(uart0_lock);
        init_lock(uart1_lock);
        uart_init(uart0);
        uart_init(uart1);
    }

    /* setup int */
    set_mtvec((any)trap_entry); // set int vec
    set_mie((get_mie() & ~(MASK_MIE_MTIE | MASK_MIE_MEIE)) |
            MASK_MIE_MSIE); // enable soft int only
    unset_msip(hartid);     // clear self soft int

    /* setup HCB */
    set_mscratch(compute_hartid2hstackbase(hartid)); // used in trap_entry
    compute_hartid2HCB(hartid)->hartid = hartid;
    init_lock(compute_hartid2HCB(hartid)->cmd.lock);

    set_mstatus(get_mstatus() | MASK_MSTATUS_MIE); // enable int
    /*********************************************************************/

    if (hartid == 0) {
        // commands
        spin2lock(&compute_hartid2HCB(3)->cmd.lock);
        set_msip(3);

        spin2lock(&compute_hartid2HCB(1)->cmd.lock);
        set_msip(1);

        spin2lock(&compute_hartid2HCB(3)->cmd.lock);
        set_msip(3);
    }

    for (;;)
        wait4int(); // wait for task
}
