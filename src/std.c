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
    char  _alc[] = "00" STR(UINT64_MAX);
    char *s      = _alc;
    for (; *s; ++s)
        *s = '0'; // write all zeros
    --s;          // move left from \0
    // for (; num && s >= _alc; --s) {
    //     *s = "0123456789abcdef"[num & 0xf];
    //     num >>= 4;
    // }
    // for (; (*(s + 2) == '0' && *(s + 3)); ++s)
    //     ;
    // s[1] = 'x';
    uart_puts(*urt, s);
}
