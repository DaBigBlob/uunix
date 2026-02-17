#ifndef UUNIX_LINKER
#define UUNIX_LINKER
#include "pre.h"

/* manual says 32bit aligned */
extern volatile u32
    __uart0[],
    __uart1[]
;

/* 64bit machine */
extern volatile usize
    __stack_base[],
    __bss[],
    __bss_end[]
;

extern volatile usize __ready[1];

#endif // UUNIX_LINKER
