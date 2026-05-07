#ifndef UUNIX_INTERR
#define UUNIX_INTERR
#include "pre.h"

/* from linker */
extern volatile u32 clint[];

#define get_msip(hartid) (clint[hartid] & 1)
#define set_msip(hartid)                                                  \
    do {                                                                  \
        clint[hartid] = 1;                                                \
    } while (0)
#define unset_msip(hartid)                                                \
    do {                                                                  \
        clint[hartid] = 0;                                                \
    } while (0)

#define MASK_MSTATUS_MIE ((usize)1 << 3) /* global interrupt enable */

#define MASK_MCAUSE_INTR ((usize)1 << ((sizeof(usize) * CHAR_BIT) - 1))

#define MASK_MIE_MSIE ((usize)1 << 3)  /* software interrupt enable */
#define MASK_MIE_MTIE ((usize)1 << 7)  /* timer interrupt enable */
#define MASK_MIE_MEIE ((usize)1 << 11) /* external interrupt enable */

void trap_handle(void);

#endif // UUNIX_INTERR
