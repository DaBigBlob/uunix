#include "std.h"
#include "uart.h"

/* not a macro because uses significant stack */
void uart_putu64(volatile uart_t *urt, u64 num)
{
    char buf[2 + ((sizeof(u64) / sizeof(u8)) * 2) + 1]; //"0x" byte*"ff" \0
    char *s = buf + sizeof(buf);

    *(--s) = '\0';

    do {
        *(--s) = "0123456789abcdef"[num & 0xf];
        num >>= 4;
    } while (num);

    *(--s) = 'x';
    *(--s) = '0';

    uart_puts(*urt, s);
}
