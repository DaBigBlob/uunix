#ifndef UUNIX_LINKER
#define UUNIX_LINKER
#include "pre.h"
#include "uart.h"
#include "clock.h"

extern volatile clck_t clckctrl;
extern volatile uart_t uart0, uart1;
extern volatile addr
    bss_begin[],
    bss_end[]
;

#endif // UUNIX_LINKER
