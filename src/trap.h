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

#define MASK_MSTATUS_MIE  ((usize)1 << 3)  /* global interrupt enable */
#define MASK_MSTATUS_MPIE ((usize)1 << 7)  /* previous int enable (mret) */
#define MASK_MSTATUS_MPP  ((usize)3 << 11) /* previous privilege (mret) */

#define CODE_MSTATUS_MPP_U ((usize)0 << 11)
#define CODE_MSTATUS_MPP_S ((usize)1 << 11)
#define CODE_MSTATUS_MPP_M ((usize)3 << 11)

#define MASK_MCAUSE_INTR    ((usize)1 << ((sizeof(usize) * CHAR_BIT) - 1))
#define MCAUSE_CODE(mcause) ((mcause) & ~MASK_MCAUSE_INTR)
#define MCAUSE_IS_INTR(mcause) ((mcause) & MASK_MCAUSE_INTR)

#define CODE_MCAUSE_INTR_SOFT     ((usize)3)
#define CODE_MCAUSE_INTR_TIMER    ((usize)7)
#define CODE_MCAUSE_INTR_EXTERNAL ((usize)11)

#define CODE_MCAUSE_EXP_INST_ADDR_MISALIGNED  ((usize)0)
#define CODE_MCAUSE_EXP_INST_ACCESS_FAULT     ((usize)1)
#define CODE_MCAUSE_EXP_ILLEGAL_INST          ((usize)2)
#define CODE_MCAUSE_EXP_BREAKPOINT            ((usize)3)
#define CODE_MCAUSE_EXP_LOAD_ADDR_MISALIGNED  ((usize)4)
#define CODE_MCAUSE_EXP_LOAD_ACCESS_FAULT     ((usize)5)
#define CODE_MCAUSE_EXP_STORE_ADDR_MISALIGNED ((usize)6)
#define CODE_MCAUSE_EXP_STORE_ACCESS_FAULT    ((usize)7)
#define CODE_MCAUSE_EXP_ECALL_U               ((usize)8)
#define CODE_MCAUSE_EXP_ECALL_S               ((usize)9)
#define CODE_MCAUSE_EXP_ECALL_M               ((usize)11)
#define CODE_MCAUSE_EXP_INST_PAGE_FAULT       ((usize)12)
#define CODE_MCAUSE_EXP_LOAD_PAGE_FAULT       ((usize)13)
#define CODE_MCAUSE_EXP_STORE_PAGE_FAULT      ((usize)15)

#define MASK_MIE_MSIE ((usize)1 << 3)  /* software interrupt enable */
#define MASK_MIE_MTIE ((usize)1 << 7)  /* timer interrupt enable */
#define MASK_MIE_MEIE ((usize)1 << 11) /* external interrupt enable */

void trap_handle(void);

#endif // UUNIX_INTERR
