#include "pre.h"
#include "std.h"
#include "uart.h"
#include "linker.h"
#include "entry.h"

static noreturn void other(void) {
    uart_puts(uart0, "hi!\r\n")
    dead_spin();
}

noreturn void main(void) {
    mem_set(bss_begin, bss_end, volatile addr, 0)
    clckctrl.coreclksel = 1; /* bypass PPL; use hardware clock @ 33MHz*/
    uart_init(uart0)

    uart_puts(uart0, "\r\nhello metal!\r\ncorepllcfg0: ")

    uint2cstr(32, clckctrl.corepllcfg0, lig)
    uart_puts(uart0, lig)

    uart_puts(uart0, "\r\ncalling other harts:\r\n")
    ready = other;

    dead_spin();
}
