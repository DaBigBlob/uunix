#include "task.h"
#include "base.h"
#include "hcb.h"
#include "mem.h"
#include "lock.h"
#include "uart.h"
#include "std.h"

/* hart tasts are executed after mret so safe to int again
(reacll trap depth = 1) */

noreturn void task_done(void)
{
    /* end of task's responsibility to say "im done" */
    dumb2unlock((compute_hartid2HCB(get_mhartid()))->cmd.lock);
    for (;;)
        wait4int();
}

/* for basic debug */
#define df0(a) u64 a
#define k      ,
noreturn void task_say_args(REGISTER_LIST_a(df0, k))
#undef k
#undef df0
{
    spin2lock(&uart0_lock);
    uart_puts(uart0, "\r\n======TASK========\r\n");
    uart_puts(uart0, "task args:");
#define df0(a)                                                            \
    uart_puts(uart0, "\r\n    " #a ":");                                  \
    uart_putu64(&uart0, a)
    REGISTER_LIST_a(df0, ;);
#undef df0
    uart_puts(uart0, "\r\n==================\r\n");
    spin2unlock(&uart0_lock);

    task_done();
}

// noreturn void task_exec_U(any code_addr, any sp)
// {
//     /** plan:
//         - in C (here):
//          - setup self mstatus.MPP, MPIE
//          - place in HCB.cmd: func=U_code, sp=sp, ra=0(return using exit sycall)
//          - set_msip(self hartid)
//      */
// }
