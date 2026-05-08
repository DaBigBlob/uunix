#include "syscall.h"
#include "lock.h"
#include "mem.h"
#include "pre.h"
#include "std.h"
#include "task.h"
#include "trap.h"
#include "uart.h"

void handle_syscall(volatile HCB *hcb)
{
    /* increment pc beyond ecall */
    hcb->mepc = (any)((usize)hcb->mepc + 4); // ecall is 32bit

    switch ((usize)hcb->a7) {
    case (SYS_WRITE):
        spin2lock(&uart0_lock);
        uart_putc(uart0, (char)(usize)(hcb->a0));
        spin2unlock(&uart0_lock);
        break;

    case (SYS_READ):
        hcb->a0 = (any)(usize)uart_getc(&uart0);
        break;

    case (SYS_EXIT):
        goto gt_task_done;

    default:
        spin2lock(&uart0_lock);
        uart_puts(uart0,
                  "\r\n======TRAP:EXP_ECALL_B/S/U(BAD_SYSCALL)\r\n");
        uart_puts(uart0, "syscall args:");

#define df0(a)                                                            \
    uart_puts(uart0, "\r\n    " #a ":");                                  \
    uart_putu64(&uart0, (u64)hcb->a)

        REGISTER_LIST_a(df0, ;);
#undef df0
        uart_puts(uart0, "\r\nWILL-NOT-RETURN-TO-CALLER");
        uart_puts(uart0, "\r\n==================\r\n");
        spin2unlock(&uart0_lock);

gt_task_done:
        /* send to task done */
        hcb->mepc    = (any)task_done; /* wait for new instructions */
        hcb->sp      = compute_hartid2HCB(hcb->hartid); /* reuse*/
        hcb->mstatus = (any)(((usize)hcb->mstatus & ~MASK_MSTATUS_MPP) |
                             CODE_MSTATUS_MPP_M |
                             MASK_MSTATUS_MPIE); /* make sure M mode */
    }
}
