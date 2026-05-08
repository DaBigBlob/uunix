#include "uart.h"

spinlock_t uart0_lock, uart1_lock;

char uart_getc(volatile uart_t *urt)
{
    u32 hld = (urt)->rxdata;
    for (; hld & MASK_UART_RX_ISEMPTY; hld = (urt)->rxdata)
        ; // busy spin bad
    return (char)hld;
}

void uart_read(volatile uart_t *urt, void *buff, usize len)
{
    len += 1;
    for (char *bu = (char *)buff; --len; ++bu)
        *bu = uart_getc(urt);
}
