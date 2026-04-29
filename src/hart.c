#include "hart.h"
#include "std.h"

addr get_HCB_addr(void)
{
    return M_get_HCB_addr(get_hartid());
}

void reset_HCB(void)
{
    ((HCB *)(M_get_HCB_addr(get_hartid())))->jump_addr = 0;
}

noreturn void hart_done(void)
{
    volatile HCB *hcb = (volatile HCB *)M_get_HCB_addr(get_hartid());

    /* wait till new task is available */
    while (hcb->jump_addr == 0)
        ;

    spin2lock(&hcb->lock);
    HCB dup;
    mem_cpy(hcb, hcb + sizeof(HCB), volatile u64, &dup); // 64bit aligned
    spin2unlock(&hcb->lock);

    hart_begin(dup.a0, dup.a1, dup.a2, dup.a3, dup.a4, dup.a5, (usize)hcb,
               dup.jump_addr);
}

void hart_task(usize hartid, usize a0, usize a1, usize a2, usize a3,
               usize a4, usize a5, addr jump_addr)
{
    volatile HCB *hcb = (volatile HCB *)M_get_HCB_addr(hartid);

    /* wait till hart is done */
    /* it is guaranteed that hcb.jump_addr = 0 after it has been
    accepted by hart*/
    while (hcb->jump_addr != 0)
        ;

    /* lock: make sure hart does not receive incomplete instruction */
    spin2lock(&hcb->lock);
    hcb->a0        = a0;
    hcb->a1        = a1;
    hcb->a2        = a2;
    hcb->a3        = a3;
    hcb->a4        = a4;
    hcb->a5        = a5;
    hcb->jump_addr = jump_addr;
    spin2unlock(&hcb->lock);
}
