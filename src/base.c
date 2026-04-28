#include "base.h"

addr get_HCB_addr(void)
{
    return M_get_HCB_addr();
}

void reset_HCB(void)
{
    ((HCB *)(M_get_HCB_addr()))->jump_addr = 0;
}

noreturn void hart_done(void)
{
    volatile HCB *hcb = (volatile HCB *)M_get_HCB_addr();
    hcb->jump_addr    = 0;
    spin2unlock(&hcb->lock);

    while (hcb->jump_addr == 0)
        ;
    spin2lock(&hcb->lock);
    hart_HCB_begin(hcb->a0, hcb->a1, hcb->a2, hcb->a3, hcb->a4, hcb->a5,
                   hcb->jump_addr);
}

void hart_task(usize a0, usize a1, usize a2, usize a3, usize a4, usize a5,
               addr jump_addr)
{
    volatile HCB *hcb = (volatile HCB *)M_get_HCB_addr();
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
