#include "base.h"

addr get_HCB_addr(void)
{
    return (addr)((char *)get_reg_sp() - sizeof(HCB));
}
