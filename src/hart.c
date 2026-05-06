#include "base.h"
#include "hart.h"

addr impl_get_HCB_addr(void)
{
    return get_HCB_addr(get_mhartid());
}
