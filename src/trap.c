#include "trap.h"
#include "base.h"
#include "hart.h"
#include "uart.h"
#include "std.h"
#include "lock.h"

#define MCAUSE_INT     ((usize)1 << 63)
#define MCAUSE_CODE(c) ((c) & ~MCAUSE_INT)

static u64 uart_lock = UNLOCKED_NUM;

void trap_handle(void)
{
    spin2lock(&uart_lock);
    usize mcause = get_mcause();
    usize mepc   = get_mepc();
    usize mtval  = get_mtval();

    uart_puts(uart0, "mcause=");
    uint2cstr(size, mcause, lig1);
    uart_puts(uart0, lig1);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, " mepc=");
    uint1cstr(size, mepc, lig1);
    uart_puts(uart0, lig1);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, " mtval=");
    uint1cstr(size, mtval, lig1);
    uart_puts(uart0, lig1);
    uart_puts(uart0, "\r\n");
    spin2unlock(&uart_lock);
}
