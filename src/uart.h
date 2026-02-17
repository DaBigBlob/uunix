#ifndef UUNIX_UART
#define UUNIX_UART

#include "pre.h"

i32 uart_getc(volatile u32 base[]);
void uart_putc(volatile u32 base[], char ch);
void uart_init(volatile u32 base[]);

#endif // UUNIX_UART
