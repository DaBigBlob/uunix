#ifndef UUNIX_HART
#define UUNIX_HART
#include "pre.h"
#include "base.h"

#define MAX_HARTS       256
#define HART_STACK_SIZE ((STACK_SIZE) / (MAX_HARTS))

addr ASMUSE_get_HCB_addr(void);

#endif // UUNIX_HART
