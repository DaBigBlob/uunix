#include "trap.h"
#include "hart.h"
#include "uart.h"
#include "std.h"

void trap_handle(void)
{
    uart_puts(uart0, "hello\r\n");
    unset_msip(mscratch2HCB()->hartid);
}
