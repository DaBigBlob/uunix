#ifndef UUNIX_LINKER
#define UUNIX_LINKER
#include "pre.h"

/* manual says 32bit aligned */
extern volatile u32
    uart0[],
    uart1[]
;

/* 64bit machine */
extern volatile u8
    bss_begin[],
    bss_end[]
;

#endif // UUNIX_LINKER
