#include "base.h"
#include "hart.h"
#include "hcb.h"

addr impl_get_HCB_addr(void)
{
    return compute_HCB_addr(get_mhartid());
}
