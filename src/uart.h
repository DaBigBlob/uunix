#ifndef UUNIX_UART
#define UUNIX_UART
#include "pre.h"

/* "The UART memory map has been designed to require
only naturally aligned 32-bit memory accesses." - fu540 manual */

typedef struct {
    u32 txdata;
    u32 rxdata;
    u32 txctrl;
    u32 rxctrl;
    u32 ie;
    u32 ip;
    u32 div;
} uart_t;

_Static_assert(offsetof(uart_t, txdata) == 0x00, "bad txdata offset");
_Static_assert(offsetof(uart_t, rxdata) == 0x04, "bad rxdata offset");
_Static_assert(offsetof(uart_t, txctrl) == 0x08, "bad txctrl offset");
_Static_assert(offsetof(uart_t, rxctrl) == 0x0C, "bad rxctrl offset");
_Static_assert(offsetof(uart_t, ie)     == 0x10, "bad ie offset");
_Static_assert(offsetof(uart_t, ip)     == 0x14, "bad ip offset");
_Static_assert(offsetof(uart_t, div)    == 0x18, "bad div offset");

#define MASK_UART_TX_ISFULL      0x80000000
#define MASK_UART_RX_ISEMPTY     0x80000000
#define MASK_UART_RX_DATA        0x000000ff

#define uart_putc(urt, ch) {\
    while (urt.txdata & MASK_UART_TX_ISFULL);\
    urt.txdata = ch;\
}

#define uart_getc(urt) (i32)(\
    (urt.rxdata & MASK_UART_RX_ISEMPTY) ?\
    (urt.rxdata & MASK_UART_RX_DATA) :\
    -1\
)

#define uart_init(urt) {\
    /* FIXME: urt.div hard calclated from device tree clock (33MHz) for 115200 */\
    urt.div = 289; /* configure baudrate */\
    urt.ie = 0; /* disable interrupts */\
    urt.txctrl = 1; /* enable TX */\
    urt.rxctrl = 1; /* enable Rx */\
}

#endif // UUNIX_UART
