#include "trap.h"
#include "base.h"
#include "lock.h"
#include "mem.h"
#include "pre.h"
#include "std.h"
#include "uart.h"

/** Plan
    in M-mode:
        harts may instruct other harts to "go do this" by:
            - acquire HCB.cmd.lock
            - fill up target hart's HCB.cmd
            - calling set_msip(target_hartid)
        after the (handling of the) task, target hart must:
            - release HCB.cmd.lock
    in U-mode:
        normal syscall using upper part of HCB as trap frame
*/

void trap_handle(void)
{
    // disable int: trap depth 1
    // no need to reenable int (we restore mstatus from HCB)
    set_mstatus(get_mstatus() & ~MASK_MSTATUS_MIE);

    HCB *hcb = compute_hartid2HCB(get_mhartid());

    spin2lock(&uart0_lock);
    uart_puts(uart0, "\r\n======TRAP========\r\n");
    uart_puts(uart0, "hartid  = ");
    uart_putu64(&uart0, (u64)hcb->hartid);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "mcause  = ");
    uart_putu64(&uart0, (u64)hcb->mcause);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "  code  = ");
    uart_putu64(&uart0, (u64)MCAUSE_CODE((usize)hcb->mcause));
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "mepc    = ");
    uart_putu64(&uart0, (u64)hcb->mepc);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "mtval   = ");
    uart_putu64(&uart0, (u64)hcb->mtval);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "mstatus = ");
    uart_putu64(&uart0, (u64)hcb->mstatus);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "mie     = ");
    uart_putu64(&uart0, (u64)get_mie());
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "------------------\r\n");
    if (MCAUSE_IS_INTR((usize)hcb->mcause)) {
        uart_puts(uart0, "INTR: ");

        switch (MCAUSE_CODE((usize)hcb->mcause)) {
        case CODE_MCAUSE_INTR_SOFT: /*************************************/
            uart_puts(uart0, "CODE_MCAUSE_INTR_SOFT (HART_TASK)\r\n");

            /* clear MSIP before ret or mret immediately traps again */
            unset_msip(hcb->hartid);

            uart_puts(uart0, "exec-ing code at:\r\n    ");
            uart_putu64(&uart0, (u64)hcb->cmd.func);
            uart_puts(uart0, "\r\nwith args:");
#define df0(a)                                                            \
    uart_puts(uart0, "\r\n    " #a ":");                                  \
    uart_putu64(&uart0, (u64)hcb->cmd.args.a)
            REGISTER_LIST_a(df0, ;);
#undef df0
            uart_puts(uart0, "\r\n");

            /* set args */
#define df0(a) hcb->a = hcb->cmd.args.a
            REGISTER_LIST_a(df0, ;);
#undef df0

            hcb->sp   = hcb->cmd.sp;
            hcb->ra   = hcb->cmd.ra;
            hcb->mepc = hcb->cmd.func;

            /* task at hcb->cmd.func should spin2unlock(&hcb->cmd.lock)
            this after itself*/
            break; /******************************************************/

        case CODE_MCAUSE_INTR_TIMER:
            uart_puts(uart0, "CODE_MCAUSE_INTR_TIMER\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_INTR_EXTERNAL:
            uart_puts(uart0, "CODE_MCAUSE_INTR_EXTERNAL\r\n");
            hcb->mepc = (any)wait4int;
            break;

        default:
            uart_puts(uart0, "UNKNOWN/RESERVED\r\n");
            hcb->mepc = (any)wait4int;
            break;
        }
    } else {
        uart_puts(uart0, "EXP: ");

        /* for all unhandled exceptions we send to intspin so hart never
        enters unrecoverable state*/

        switch (MCAUSE_CODE((usize)hcb->mcause)) {
        case CODE_MCAUSE_EXP_ECALL_U: /***********************************/
            uart_puts(uart0, "CODE_MCAUSE_EXP_ECALL_U\r\n");

            /* increment pc beyond ecall */
            hcb->mepc = (any)((usize)hcb->mepc + 4); // ecall is 32bit

            uart_puts(uart0, "\r\nsyscall args:");
#define df0(a)                                                            \
    uart_puts(uart0, "\r\n    " #a ":");                                  \
    uart_putu64(&uart0, (u64)hcb->a)
            REGISTER_LIST_a(df0, ;);
#undef df0

            // TODO
            break; /******************************************************/

        case CODE_MCAUSE_EXP_INST_ADDR_MISALIGNED:
            uart_puts(uart0, "CODE_MCAUSE_EXP_INST_ADDR_MISALIGNED\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_INST_ACCESS_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_INST_ACCESS_FAULT\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_ILLEGAL_INST:
            uart_puts(uart0, "CODE_MCAUSE_EXP_ILLEGAL_INST\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_BREAKPOINT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_BREAKPOINT\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_LOAD_ADDR_MISALIGNED:
            uart_puts(uart0, "CODE_MCAUSE_EXP_LOAD_ADDR_MISALIGNED\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_LOAD_ACCESS_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_LOAD_ACCESS_FAULT\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_STORE_ADDR_MISALIGNED:
            uart_puts(uart0, "CODE_MCAUSE_EXP_STORE_ADDR_MISALIGNED\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_STORE_ACCESS_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_STORE_ACCESS_FAULT\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_ECALL_S:
            uart_puts(uart0, "CODE_MCAUSE_EXP_ECALL_S\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_ECALL_M:
            uart_puts(uart0, "CODE_MCAUSE_EXP_ECALL_M\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_INST_PAGE_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_INST_PAGE_FAULT\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_LOAD_PAGE_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_LOAD_PAGE_FAULT\r\n");
            hcb->mepc = (any)wait4int;
            break;

        case CODE_MCAUSE_EXP_STORE_PAGE_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_STORE_PAGE_FAULT\r\n");
            hcb->mepc = (any)wait4int;
            break;

        default:
            uart_puts(uart0, "UNKNOWN/RESERVED\r\n");
            hcb->mepc = (any)wait4int;
            break;
        }
    }

    uart_puts(uart0, "==================\r\n");
    spin2unlock(&uart0_lock);
}
