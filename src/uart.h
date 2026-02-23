#ifndef UUNIX_UART
#define UUNIX_UART
#include "pre.h"
#include <stdalign.h>

/* "The UART memory map has been designed to require
only naturally aligned 32-bit memory accesses." - fu540 manual */

typedef struct {
    alignas(alignof(u32)) u32 txdata;
    alignas(alignof(u32)) u32 rxdata;
    alignas(alignof(u32)) u32 txctrl;
    alignas(alignof(u32)) u32 rxctrl;
    alignas(alignof(u32)) u32 ie;
    alignas(alignof(u32)) u32 ip;
    alignas(alignof(u32)) u32 div;
} uart_t;

#define UART_TXFIFO_ISFULL      0x80000000
#define UART_RXDATA_ISEMPTY     0x80000000
#define UART_RXDATA_DATA        0x000000ff
#define UART_TXCTRL_TXEN        0x1
#define UART_RXCTRL_RXEN        0x1

#define uart_putc(urt, ch) {\
    while (urt.txdata & UART_TXFIFO_ISFULL);\
    urt.txdata = ch;\
}

#define uart_getc(urt) (i32)(\
    (urt.rxdata & UART_RXDATA_ISEMPTY) ?\
    (urt.rxdata & UART_RXDATA_DATA) :\
    -1\
)

#define uart_init(urt) {\
    /* FIXME: urt.div hard calclated from device tree clock (33MHz) for 115200 */\
    urt.div = 289; /* configure baudrate */\
    urt.ie = 0; /* disable interrupts */\
    urt.txctrl = UART_TXCTRL_TXEN; /* enable TX */\
    urt.rxctrl = UART_RXCTRL_RXEN; /* enable Rx */\
}

#endif // UUNIX_UART
