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
    uart_init(uart0)

    uart_puts(uart0, "\r\nhello metal!\r\n")

    // uint2cstr(8, 69, lig)
    // uart_puts(uart0, lig)

    uart_puts(uart0, "\r\ncalling other harts:\r\n")
    ready = other;

    dead_spin();
}
