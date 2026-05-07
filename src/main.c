#include "pre.h"
#include "base.h"
#include "trap.h"
#include "mem.h"
#include "std.h"

volatile static u8 init_done = 0; // only hart 0 should mutate

noreturn void main(void)
{
    /* inits hart 0 needs to do */
    if (get_mhartid() == 0) {
        mem_set(bss_begin, bss_end, volatile u8, 0);
        uart_init(uart0);
        uart_init(uart1);
        init_done = 1;
    } else {
        while (!init_done)
            ; // dumb wait: stack/memory not safe
    }

    // int still disabled
    /*********************************************************************/
    set_mstatus(get_mstatus() & ~MASK_MSTATUS_MIE); // disable int

    usize hartid = get_mhartid();

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

    while (!init_done)
        ; // dumb wait: nothing else safe yet

    if (hartid == 0) {
        // command
    }

    for (;;)
        wait4int(); // wait for task
}
