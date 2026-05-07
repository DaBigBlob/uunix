#include "task.h"

/* hart tasts are executed after mret so safe to int again */

noreturn void task_exec_U(any code_addr, any sp)
{
    for (;;)
        ;
}
