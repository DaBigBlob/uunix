#include "task.h"
#include "base.h"
#include "hcb.h"
#include "mem.h"
#include "uart.h"
#include "std.h"

/* hart tasts are executed after mret so safe to int again
(reacll trap depth = 1) */

/* for basic debug */
#define df0(a) u64 a
#define k      ,
noreturn void task_say_args(REGISTER_LIST_a(df0, k))
#undef k
#undef df0
{
    uart_puts(uart0, "\r\nsyscall args:");
#define df0(a)                                                            \
    uart_puts(uart0, "\r\n    " #a ":");                                  \
    uart_putu64(&uart0, a)
    REGISTER_LIST_a(df0, ;);
#undef df0

    for (;;)
        wait4int();
}

static noreturn void task_exec_U_init(any code_addr, any sp)
{
    HCB *hcb = compute_hartid2HCB(get_mhartid());
    for (;;)
        wait4int();
}
