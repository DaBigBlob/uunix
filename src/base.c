#include "base.h"
#include "mem.h"

any ASMUSE_get_hstackbase(void)
{
    return compute_hartid2hstackbase(get_mhartid());
}
