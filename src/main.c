#include "main.h"
#include "std.h"
#include "uart.h"
#include "linker.h"

void other(void) {
    for (char* say = "hi!\n"; *say; ++say)
        uart_putc(__uart0, *say);
}

void __main(void) {
    memset(__bss, __bss_end, 0);
    uart_init(__uart0);

    for (char* say = "\nhello metal!\n"; *say; ++say)
        uart_putc(__uart0, *say);

    for (char* say = "\ncalling others:\n"; *say; ++say)
        uart_putc(__uart0, *say);
    __ready[0] = (usize)other;
}
