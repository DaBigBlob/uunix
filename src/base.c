#include "base.h"

addr get_HCB_addr(void)
{
    return (addr)((usize)get_reg_sp() - sizeof(HCB));
}
