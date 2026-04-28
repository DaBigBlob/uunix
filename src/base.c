#include "base.h"

addr get_HCB_addr(void)
{
    return (addr)((u8 *)get_reg_sp() - sizeof(HCB));
}
