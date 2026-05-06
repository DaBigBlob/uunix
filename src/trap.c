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
    unset_msip(get_mhartid());

    spin2lock(&uart_lock);

    uart_puts(uart0, "hartid=");
    uart_putu64(&uart0, get_mhartid());
    uart_puts(uart0, "\r\n");

    usize mcause = get_mcause();
    usize mepc   = get_mepc();
    usize mtval  = get_mtval();

    uart_puts(uart0, "mcause=");
    uart_putu64(&uart0, mcause);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "mepc=");
    uart_putu64(&uart0, mepc);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "mtval=");
    uart_putu64(&uart0, mtval);
    uart_puts(uart0, "\r\n");

    // uart_puts(uart0, "hartid=");
    // uart_putu64(&uart0, get_mhartid());
    // uart_puts(uart0, "\r\n");

    uart_puts(uart0, "---------\r\n");
    spin2unlock(&uart_lock);
}
