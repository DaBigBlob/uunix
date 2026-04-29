#include "base.h"
#include "pre.h"
#include "std.h"
#include "uart.h"
#include "hart.h"
#include "conv.h"

static noreturn void task1(void)
{
    say_hart(uart0);
    static u64 lock = new_spinlock();
    spin2lock(&lock);
    uart_puts(uart0, "hi!\r\n");
    spin2unlock(&lock);
    hart_done();
}

noreturn void main(void)
{
    // ((HCB *)M_get_HCB_addr(get_hartid()))->jump_addr = 0; // clean a little
    if (get_hartid() != 0)
        hart_done(); // send non-0 harts to spin-wait

    /* setup */
    mem_set(bss_begin, bss_end, volatile addr, 0);
    uart_init(uart0);

    // hi
    say_hart(uart0);
    uart_puts(uart0, "hello metal!\r\n");

    say_hart(uart0);
    uart_puts(uart0, "hart 0 hcb: ");
    uint2cstr(64, get_HCB_addr(), lig);
    uart_puts(uart0, lig);
    uart_puts(uart0, "\r\n");

    say_hart(uart0);
    uart_puts(uart0, "hart 1 hcb: ");
    uint2cstr(64, M_get_HCB_addr(1), lig2);
    uart_puts(uart0, lig2);
    uart_puts(uart0, "\r\n");

    say_hart(uart0);
    uart_puts(uart0, "setting task for other hart 1...\r\n");

    hart_task(1, 0, 0, 0, 0, 0, 0, (addr)task1);

    dead_spin();
}
