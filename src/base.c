#include "base.h"
#include "mem.h"

any ASMUSE_get_hstackbase(void)
{
    return compute_hstackbase(get_mhartid());
}
