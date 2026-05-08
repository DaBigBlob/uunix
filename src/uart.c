#include "uart.h"

spinlock_t uart0_lock, uart1_lock;

char uart_getc(volatile uart_t *urt)
{
    u32 hld = (urt)->rxdata;
    for (; hld & MASK_UART_RX_ISEMPTY; hld = (urt)->rxdata)
        ;
    return (char)hld;
}
