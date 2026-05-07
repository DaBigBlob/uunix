#include "task.h"
#include "base.h"
#include "mem.h"

/* hart tasts are executed after mret so safe to int again
(reacll trap depth = 1) */

static noreturn void task_exec_U_init(any code_addr, any sp)
{
    HCB *hcb = compute_hartid2HCB(get_mhartid());
}
