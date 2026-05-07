#include "pre.h"
#include "base.h"
#include "trap.h"
#include "mem.h"
#include "uart.h"

noreturn void main(void)
{
    /* pre mem touch begin: unsafe to touch memory ***********************/
    {
        /*
        * Situation:
            We don't want to touch stack yet because its probably in .bss
            (which is what we are trying to zero).
        * Remark:
            Compiler should use registers only for this section but we
            can't guarantee that from C.
        * Solution:
            We try our best to convince the compiler to put the variables
            in this section in .data by assigning values to them.
            'volatile static' should prevent further optimization attempts.
        */
        volatile static u8 init_pending = 0xf; // only hart 0 should mutate
        /* inits hart 0 needs to do */
        if (get_mhartid() == 0) {
            {
                volatile static u8 *_b = (u8 *)0xa;
                for (_b = bss_begin; _b < bss_end; ++_b) {
                    *_b = 0;
                }
            }
            init_pending = 0;
        } else {
            while (init_pending)
                ; // dumb wait: stack/memory not safe
        }
    }
    /* pre mem touch end: unsafe to touch memory *************************/

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
