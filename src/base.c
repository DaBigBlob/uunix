#include "base.h"

addr get_HCB_addr(void)
{
    return M_get_HCB_addr();
}

void reset_HCB(void)
{
    ((HCB *)(M_get_HCB_addr()))->jump_addr = 0;
}
