#ifndef UUNIX_UART
#define UUNIX_UART
#include "pre.h"
#include "clock.h"

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

#define check_offset(n, o)                                                \
    _Static_assert(offsetof(uart_t, n) == o, "bad " #n " offset")
check_offset(txdata, 0x00);
check_offset(rxdata, 0x04);
check_offset(txctrl, 0x08);
check_offset(rxctrl, 0x0C);
check_offset(ie, 0x10);
check_offset(ip, 0x14);
check_offset(div, 0x18);
#undef check_offset

extern volatile uart_t uart0, uart1;

#define MASK_UART_TX_ISFULL  0x80000000
#define MASK_UART_RX_ISEMPTY 0x80000000
#define MASK_UART_RX_DATA    0x000000ff

#define uart_putc(urt, ch)                                                \
    do {                                                                  \
        while (urt.txdata & MASK_UART_TX_ISFULL)                          \
            ;                                                             \
        urt.txdata = ch;                                                  \
    } while (0)

#define uart_getc(urt)                                                    \
    (i32)((urt.rxdata & MASK_UART_RX_ISEMPTY) ?                           \
              (urt.rxdata & MASK_UART_RX_DATA) :                          \
              -1)

#define BAUDRATE 115200

#define uart_init(urt)                                                    \
    do {                                                                  \
        /* uart runs on tilelink clock which is always half of coreclk */ \
        urt.div    = (TLCLK_HZ() / BAUDRATE); /* configure baudrate */    \
        urt.ie     = 0;                       /* disable interrupts */    \
        urt.txctrl = 1;                       /* enable TX */             \
        urt.rxctrl = 1;                       /* enable Rx */             \
    } while (0)

#endif // UUNIX_UART
