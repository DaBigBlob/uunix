#include "lock.h"
#include "pre.h"
#include "base.h"
#include "task.h"
#include "trap.h"
#include "mem.h"
#include "uart.h"

noreturn void main(void)
{
    set_mstatus(get_mstatus() & ~MASK_MSTATUS_MIE); // disable int

    usize hartid = get_mhartid();

    if (hartid == 0) {
        init_spinlock(uart0_lock);
        init_spinlock(uart1_lock);
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
    init_dumblock(compute_hartid2HCB(hartid)->cmd.lock);

    set_mstatus(get_mstatus() | MASK_MSTATUS_MIE); // enable int
    /*********************************************************************/

    if (hartid == 0) {
        // commands
        dumb2lock(compute_hartid2HCB(3)->cmd.lock);
        compute_hartid2HCB(3)->cmd.mpp = CODE_MSTATUS_MPP_M;
        set_msip(3); // should fault

        dumb2lock(compute_hartid2HCB(3)->cmd.lock);
        compute_hartid2HCB(3)->cmd.func    = (any)task_say_args;
        compute_hartid2HCB(3)->cmd.args.a0 = (any)0xc;
        compute_hartid2HCB(3)->cmd.args.a1 = (any)3;
        compute_hartid2HCB(3)->cmd.sp      = compute_hartid2HCB(3);
        compute_hartid2HCB(3)->cmd.mpp     = CODE_MSTATUS_MPP_M;
        set_msip(3); // should recover (it does: commit ace9a8d)
    }

    for (;;)
        wait4int(); // wait for task
}
