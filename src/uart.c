#include "uart.h"

/* "The UART memory map has been designed to require
only naturally aligned 32-bit memory accesses." - fu540 manual */

#define UART_REG_TXFIFO         0
#define UART_REG_RXFIFO         1
#define UART_REG_TXCTRL         2
#define UART_REG_RXCTRL         3
#define UART_REG_IE             4
#define UART_REG_IP             5
#define UART_REG_DIV            6

#define UART_TXFIFO_FULL        0x80000000
#define UART_RXFIFO_EMPTY       0x80000000
#define UART_RXFIFO_DATA        0x000000ff
#define UART_TXCTRL_TXEN        0x1
#define UART_RXCTRL_RXEN        0x1

void uart_putc(volatile u32 base[], char ch) {
    while (base[UART_REG_TXFIFO] & UART_TXFIFO_FULL);
    base[UART_REG_TXFIFO] = ch;
}

i32 uart_getc(volatile u32 base[]) {
    u32 ret = base[UART_REG_RXFIFO];
    if (!(ret & UART_RXFIFO_EMPTY))
        return ret & UART_RXFIFO_DATA;
    else
        return -1;
}

void uart_init(volatile u32 base[]) {
    /* FIXME: hard calclated from device tree clock (33MHz) for 115200 */
    base[UART_REG_DIV] = 289; /* configure baudrate */

    base[UART_REG_IE] = 0; /* disable interrupts */
    base[UART_REG_TXCTRL] = UART_TXCTRL_TXEN; /* enable TX */
    base[UART_REG_RXCTRL] = UART_RXCTRL_RXEN; /* enable Rx */
}
