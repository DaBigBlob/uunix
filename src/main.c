#include "base.h"
#include "pre.h"
#include "std.h"
#include "uart.h"
#include "hart.h"
#include "task.h"
#include "interr.h"

noreturn void main(void)
{
    /* setup interrupts on all harts */
    set_mtvec(hatwait);                            // clear
    set_mie(get_mie() | MASK_MIE_MSIE);            // enable software int
    set_mstatus(get_mstatus() | MASK_MSTATUS_MIE); // enable global int

    /* send non-0 harts to spin-wait */
    if (get_mhartid() != 0) {
        HCB_set_fld(get_mhartid(), jump_addr) = hatwait;
        hart_done();
    }

    /* setup */
    mem_set(bss_begin, bss_end, volatile addr, 0);
    uart_init(uart0);
    uart_init(uart1);

    /* set task for self */
    hart_task(0, 0, 0, 0, 0, 0, 0, (any)h0t0);
    hart_done();
}
