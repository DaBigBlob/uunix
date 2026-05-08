#include "mem.h"

alignas(16) volatile khart_mem_t kern_mem[MAX_HARTS];
