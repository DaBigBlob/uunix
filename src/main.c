#include "std.h"
#include "uart.h"
#include "linker.h"
#include "entry.h"

static void other(void) {
    for (char* say = "hi!\n"; *say; ++say)
        uart_putc(uart0, *say);
}

extern void main(void) {
    memset(volatile u8*, bss_begin, bss_end, 0)
    uart_init(uart0);

    for (char* say = "\nhello metal!\n"; *say; ++say)
        uart_putc(uart0, *say);

    for (char* say = "\ncalling others:\n"; *say; ++say)
        uart_putc(uart0, *say);

    ready = other;
}
