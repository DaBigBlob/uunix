#ifndef UUNIX_LINKER
#define UUNIX_LINKER
#include "pre.h"
#include "uart.h"

extern volatile uart_t uart0, uart1;

/* 64bit machine */
extern volatile addr
    bss_begin[],
    bss_end[]
;

#endif // UUNIX_LINKER
