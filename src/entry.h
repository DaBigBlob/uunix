#ifndef UUNIX_ENTRY
#define UUNIX_ENTRY
#include "pre.h"

extern void (*volatile ready)(void);
extern noreturn void dead_spin(void);

extern volatile addr
    heap_top[],
    stack_base[]
;

#endif // UUNIX_ENTRY
