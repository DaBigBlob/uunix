#ifndef UUNIX_MEM
#define UUNIX_MEM
#include "hcb.h"

#define STACK_SIZE      1048576 /* 1MiB */
#define MAX_HARTS       128
#define HART_STACK_SIZE ((STACK_SIZE) / (MAX_HARTS))

typedef struct {
    u8 stacktop[HART_STACK_SIZE];
    //  <-- stackbase starts at hcb and grows ^
    // below it HCB
    HCB hcb;
} hart_mem_t;

#define compute_hartid2HCB(hartid)        (&kern_mem[(hartid)].hcb)
#define compute_hartid2hstackbase(hartid) ((any)compute_hartid2HCB(hartid))
#define compute_hstackbase2HCB(hstackbase) ((HCB *)(hstackbase))

alignas(16) extern hart_mem_t kern_mem[MAX_HARTS];

/* from linker */
extern volatile u8 bss_begin[], bss_end[];

#endif // UUNIX_MEM
