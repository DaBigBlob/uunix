#ifndef UUNIX_CONV
#define UUNIX_CONV

/* convenience stuff */

#include "uart.h"
#include "hart.h"

#define say_hart(urt, str)                                                \
    do {                                                                  \
        uart_puts(urt, "[hart:");                                         \
        uint2cstr(64, get_hartid(), _uunix_priv___say_hart_v0);           \
        uart_puts(urt, _uunix_priv___say_hart_v0);                        \
        uart_puts(urt, "] ");                                             \
        uart_puts(urt, str);                                              \
    } while (0)

#endif // UUNIX_CONV
