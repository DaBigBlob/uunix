#include "task.h"
#include "base.h"
#include "pre.h"
#include "std.h"
#include "uart.h"
#include "hart.h"
#include "conv.h"

static u64 uart_lock = new_spinlock();

static noreturn void hat0(usize a0, usize a1, usize a2, usize a3)
{
    spin2lock(&uart_lock);

    say_hart(uart0);
    uart_puts(uart0, "begin hart:any,task:0\r\n");

    say_hart(uart0);
    uart_puts(uart0, "my reg a0 is:");
    uint2cstr(64, a0, lig);
    uart_puts(uart0, lig);
    uart_puts(uart0, "\r\n");

    say_hart(uart0);
    uart_puts(uart0, "my reg a1 is:");
    uint2cstr(64, a1, lig1);
    uart_puts(uart0, lig1);
    uart_puts(uart0, "\r\n");

    say_hart(uart0);
    uart_puts(uart0, "my reg a2 is:");
    uint2cstr(64, a2, lig2);
    uart_puts(uart0, lig2);
    uart_puts(uart0, "\r\n");

    say_hart(uart0);
    uart_puts(uart0, "my reg a3 is:");
    uint2cstr(64, a3, lig3);
    uart_puts(uart0, lig3);
    uart_puts(uart0, "\r\n");

    spin2unlock(&uart_lock);
    hart_done();
}

noreturn void h0t0(void)
{
    spin2lock(&uart_lock);

    say_hart(uart0);
    uart_puts(uart0, "begin hart:0,task:0\r\n");

    say_hart(uart0);
    uart_puts(uart0, "hello metal!\r\n");

    say_hart(uart0);
    uart_puts(uart0, "TASK TESTING HARTS: 2, 3\r\n");

    say_hart(uart0);
    uart_puts(uart0, "hart 0 hcb:");
    uint2cstr(64, M_get_HCB_addr(0), lig);
    uart_puts(uart0, lig);
    uart_puts(uart0, "\r\n");

    say_hart(uart0);
    uart_puts(uart0, "hart 2 hcb:");
    uint2cstr(64, M_get_HCB_addr(2), lig2);
    uart_puts(uart0, lig2);
    uart_puts(uart0, "\r\n");
    say_hart(uart0);
    uart_puts(uart0, "hart 3 hcb:");
    uint2cstr(64, M_get_HCB_addr(3), lig3);
    uart_puts(uart0, lig3);
    uart_puts(uart0, "\r\n");

    say_hart(uart0);
    uart_puts(uart0,
              "setting task for other harts 2 and 3 then 2 again...\r\n");

    spin2unlock(&uart_lock);

    hart_task(2, 0, 1, 2, 3, 4, 5, (addr)hat0);
    hart_task(3, 1, 2, 3, 4, 5, 0, (addr)hat0);
    hart_task(2, 2, 3, 4, 5, 0, 1, (addr)hat0);

    hart_done();
}
