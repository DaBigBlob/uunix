#include "base.h"
#include "hart.h"

addr impl_get_HCB_addr(void)
{
    return compute_HCB_addr(get_mhartid());
}
