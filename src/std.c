#include "std.h"
#include "uart.h"

#define uint1cstr(size, _num, id)                                         \
    do {                                                                  \
        char   *cur = id;                                                 \
        u##size num = (u##size)(_num);                                    \
        for (; *cur; ++cur)                                               \
            *cur = '0';                                                   \
        for (--cur; num && cur >= id; --cur) {                            \
            *cur = "0123456789abcdef"[num & 0xf];                         \
            num >>= 4;                                                    \
        }                                                                 \
        for (; (*(id + 2) == '0' && *(id + 3)); ++id)                     \
            ;                                                             \
        id[1] = 'x';                                                      \
    } while (0)

#define uint2cstr(size, _num, id)                                         \
    char  _uunix_priv__uint2cstr##id[] = "00" STR(UINT##size##_MAX);      \
    char *id                           = _uunix_priv__uint2cstr##id;      \
    uint1cstr(size, _num, id)

void uart_putu64(volatile uart_t *urt, u64 num)
{
    char  buf[2 + 16 + 1];
    char *s = buf + sizeof buf;

    *(--s) = '\0';

    do {
        *(--s) = "0123456789abcdef"[num & 0xf];
        num >>= 4;
    } while (num);

    *(--s) = 'x';
    *(--s) = '0';

    uart_puts(*urt, s);
}
