#ifndef UUNIX_BASE
#define UUNIX_BASE
#include "pre.h"

extern volatile addr bss_begin[], bss_end[];

extern void (*volatile ready)(void);
extern noreturn void dead_spin(void);

extern volatile addr kheap_top[], kstack_base[];

#endif // UUNIX_BASE
