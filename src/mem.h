#ifndef UUNIX_MEM
#define UUNIX_MEM
#include "hcb.h"

#define KMEM_SIZE      1048576 /* 1MiB */
#define MAX_HARTS      128
#define HART_KMEM_SIZE (((KMEM_SIZE) / (MAX_HARTS)) - sizeof(HCB))

typedef struct {
    u8 stacktop[HART_KMEM_SIZE];
    //  <-- stackbase starts at hcb and grows ^
    // below it HCB
    HCB hcb;
} khart_mem_t;

#define compute_hartid2HCB(hartid)        (&kern_mem[(hartid)].hcb)
#define compute_hartid2hstackbase(hartid) ((any)compute_hartid2HCB(hartid))
#define compute_hstackbase2HCB(hstackbase) ((volatile HCB *)(hstackbase))

alignas(16) extern volatile khart_mem_t kern_mem[MAX_HARTS];

/* from linker */
extern volatile u8 bss_begin[], bss_end[];

// typedef struct {
//     u8 stacktop[HART_KMEM_SIZE];
//     //  <-- stackbase starts at hcb and grows ^
//     // below it HCB
//     struct {
//     } stuff;
// } uhart_mem_t;

#endif // UUNIX_MEM
