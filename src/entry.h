#ifndef UUNIX_ENTRY
#define UUNIX_ENTRY
#include "pre.h"

extern void (*volatile ready)(void);
extern noreturn void dead_spin(void);

#endif // UUNIX_ENTRY
