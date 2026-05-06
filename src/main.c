#include "base.h"
#include "pre.h"
#include "std.h"
#include "uart.h"
#include "hcb.h"
#include "hart.h"
#include "trap.h"

noreturn void main(void)
{
    /* setup */
    mem_set(bss_begin, bss_end, volatile addr, 0);
    uart_init(uart0);
    uart_init(uart1);

    /* set up HCB */
    set_mscratch((any)compute_HCB_addr(get_mhartid()));
    mscratch2HCB()->hartid = get_mhartid();

    /* setup interrupts on all harts */
    set_mtvec((any)trap_entry);
    set_mie((get_mie() & (~MASK_MIE_MTIE)) | MASK_MIE_MEIE |
            MASK_MIE_MSIE); // enable software+hardware int only
    set_mstatus(get_mstatus() | MASK_MSTATUS_MIE); // enable global int

    /* send non-0 harts to run once then dead spin */
    if (get_mhartid() != 0) {
        set_msip(mscratch2HCB()->hartid);
        for (;;)
            ;
    }

    uart_puts(uart0, "hi\r\n");

    /* set task for self */
    set_msip(mscratch2HCB()->hartid);

    for (;;)
        ;
}
