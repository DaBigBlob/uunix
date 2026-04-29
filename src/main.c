#include "base.h"
#include "pre.h"
#include "std.h"
#include "uart.h"
#include "hart.h"
#include "task.h"

noreturn void main(void)
{
    /* send non-0 harts to spin-wait */
    if (get_hartid() != 0) {
        ((HCB *)M_get_HCB_addr(get_hartid()))->jump_addr = 0;
        hart_done();
    }

    /* setup */
    mem_set(bss_begin, bss_end, volatile addr, 0);
    uart_init(uart0);
    uart_init(uart1);

    /* set task for self */
    hart_task(0, 0, 0, 0, 0, 0, 0, (addr)h0t0);
    hart_done();
}
