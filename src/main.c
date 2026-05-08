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
    // hart_ready[hartid] = 0; /* not ready */

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

    /* setup U mode mem access */
    // TODO: dont be retarded
    set_pmpaddr0(~(usize)0);
    set_pmpcfg0((usize)0x1f); /* R | W | X | NAPOT */

    set_mstatus(get_mstatus() | MASK_MSTATUS_MIE); // enable int

    /* only to be written by HCB owner hart once; guarantees consistency */
    compute_hartid2HCB(hartid)->init_ok = 1;
    fence_mem();
    /*********************************************************************/

    if (hartid == 0) {
        //make sure the harts we need are ready
        while (!compute_hartid2HCB(1)->init_ok)
            ;

        //commands
        dumb2lock(compute_hartid2HCB(1)->cmd.lock);
        compute_hartid2HCB(1)->cmd.func = (any)umode_test;
        compute_hartid2HCB(1)->cmd.sp   = compute_hartid2HCB(1); //for test
        compute_hartid2HCB(1)->cmd.mpp  = CODE_MSTATUS_MPP_U;
        set_msip(1);

        dumb2lock(compute_hartid2HCB(1)->cmd.lock);
        compute_hartid2HCB(1)->cmd.func    = (any)task_say_args;
        compute_hartid2HCB(1)->cmd.args.a0 = (any)0xc;
        compute_hartid2HCB(1)->cmd.args.a1 = (any)1;
        compute_hartid2HCB(1)->cmd.sp      = compute_hartid2HCB(1);
        compute_hartid2HCB(1)->cmd.mpp     = CODE_MSTATUS_MPP_M;
        set_msip(1);
    }

    task_done();
}
