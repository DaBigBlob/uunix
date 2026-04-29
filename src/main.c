#include "pre.h"
#include "std.h"
#include "uart.h"
#include "hart.h"

static noreturn void task1(void)
{
    static u64 lock = new_spinlock();
    spin2lock(&lock);
    uart_puts(uart0, "hi!\r\n");
    spin2unlock(&lock);
    hart_done();
}

noreturn void main(void)
{
    ((HCB *)M_get_HCB_addr(get_hartid()))->jump_addr = 0; // clean a little
    if (get_hartid() != 0)
        hart_done(); // send non-0 harts to spin-wait

    /* setup */
    mem_set(bss_begin, bss_end, volatile addr, 0);
    uart_init(uart0);

    // hi
    uart_puts(uart0, "\r\nhello metal!\r\n");

    uart_puts(uart0, "corepllcfg0: ");
    uint2cstr(32, clckctrl.corepllcfg0, lig);
    uart_puts(uart0, lig);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "setting task for other harts...\r\n");
    hart_task(1, 0, 0, 0, 0, 0, 0, (addr)task1);

    dead_spin();
}
