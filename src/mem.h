#ifndef UUNIX_MEM
#define UUNIX_MEM
#include "hcb.h"

#define KMEM_SIZE      1048576 /* 1MiB */
#define MAX_HARTS      8
#define HART_KMEM_SIZE (((KMEM_SIZE) / (MAX_HARTS)) - sizeof(HCB))

typedef struct {
    alignas(16) u8 stacktop[HART_KMEM_SIZE];
    //  <-- stackbase starts at hcb and grows ^
    // below it HCB
    alignas(16) HCB hcb;
} khart_mem_t;

#define compute_hartid2HCB(hartid)        (&kern_mem[(hartid)].hcb)
#define compute_hartid2hstackbase(hartid) ((any)compute_hartid2HCB(hartid))
#define compute_hstackbase2HCB(hstackbase) ((volatile HCB *)(hstackbase))

alignas(16) extern volatile khart_mem_t kern_mem[MAX_HARTS];

/* from linker */
extern volatile u8 bss_begin[], bss_end[];

#define MASK_PMP_R     ((usize)1 << 0)
#define MASK_PMP_W     ((usize)1 << 1)
#define MASK_PMP_X     ((usize)1 << 2)
#define MASK_PMP_A_TOR ((usize)1 << 3)

#define MASK_PMP_RWX_TOR                                                  \
    (MASK_PMP_R | MASK_PMP_W | MASK_PMP_X | MASK_PMP_A_TOR)

typedef struct {
    u8 stacktop[HART_KMEM_SIZE];
    //  <-- stackbase starts and grows ^
} uhart_mem_t;

#endif // UUNIX_MEM
