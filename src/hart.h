#ifndef UUNIX_HART
#define UUNIX_HART
#include "pre.h"
#include "base.h"
#include "hcb.h"

#define MAX_HARTS       256
#define HART_STACK_SIZE ((STACK_SIZE) / (MAX_HARTS))

#define compute_HCB_addr(hartid)                                          \
    ((volatile addr)(kstack_base - (HART_STACK_SIZE * (hartid + 1)) -     \
                     sizeof(HCB)))

#define mscratch2HCB() ((HCB *)get_mscratch())

addr ASMUSE_get_HCB_addr(void);

#endif // UUNIX_HART
