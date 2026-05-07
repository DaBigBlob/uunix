#include "trap.h"
#include "base.h"
#include "lock.h"
#include "mem.h"
#include "std.h"

/** Plan
    in M-mode:
        hart0 may use HCB to instruct harts to execute "tasks" by:
            - setting:
                - target code address: t0
                - sp to be set for target: t1
                - args to be passed to target: a0-a7
            -
*/

void trap_handle(void)
{
    HCB *hcb = compute_hstackbase2HCB(get_mscratch_hstackbase());

    spin2lock(&uart_lock);
    uart_puts(uart0, "\r\n==================\r\n");
    uart_puts(uart0, "hcbhart = ");
    uart_putu64(&uart0, (u64)hcb->hartid);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "mcause  = ");
    uart_putu64(&uart0, (u64)hcb->mcause);
    uart_puts(uart0, "\r\n");

    uart_puts(uart0, "  code  = ");
    uart_putu64(&uart0, (u64)MCAUSE_CODE(hcb->mcause));
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
    if (MCAUSE_IS_INTR(hcb->mcause)) {
        uart_puts(uart0, "INTR: ");

        switch (MCAUSE_CODE(hcb->mcause)) {
        case CODE_MCAUSE_INTR_SOFT:
            uart_puts(uart0, "CODE_MCAUSE_INTR_SOFT\r\n");
            /* clear MSIP before ret or mret immediately traps again */
            unset_msip(hcb->hartid);
            break;

        case CODE_MCAUSE_INTR_TIMER:
            uart_puts(uart0, "CODE_MCAUSE_INTR_TIMER\r\n");
            break;

        case CODE_MCAUSE_INTR_EXTERNAL:
            uart_puts(uart0, "CODE_MCAUSE_INTR_EXTERNAL\r\n");
            break;

        default:
            uart_puts(uart0, "UNKNOWN/RESERVED\r\n");
            break;
        }
    } else {
        uart_puts(uart0, "EXP: ");

        switch (MCAUSE_CODE(hcb->mcause)) {
        case CODE_MCAUSE_EXP_INST_ADDR_MISALIGNED:
            uart_puts(uart0, "CODE_MCAUSE_EXP_INST_ADDR_MISALIGNED\r\n");
            break;

        case CODE_MCAUSE_EXP_INST_ACCESS_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_INST_ACCESS_FAULT\r\n");
            break;

        case CODE_MCAUSE_EXP_ILLEGAL_INST:
            uart_puts(uart0, "CODE_MCAUSE_EXP_ILLEGAL_INST\r\n");
            break;

        case CODE_MCAUSE_EXP_BREAKPOINT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_BREAKPOINT\r\n");
            break;

        case CODE_MCAUSE_EXP_LOAD_ADDR_MISALIGNED:
            uart_puts(uart0, "CODE_MCAUSE_EXP_LOAD_ADDR_MISALIGNED\r\n");
            break;

        case CODE_MCAUSE_EXP_LOAD_ACCESS_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_LOAD_ACCESS_FAULT\r\n");
            break;

        case CODE_MCAUSE_EXP_STORE_ADDR_MISALIGNED:
            uart_puts(uart0, "CODE_MCAUSE_EXP_STORE_ADDR_MISALIGNED\r\n");
            break;

        case CODE_MCAUSE_EXP_STORE_ACCESS_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_STORE_ACCESS_FAULT\r\n");
            break;

        case CODE_MCAUSE_EXP_ECALL_U:
            uart_puts(uart0, "CODE_MCAUSE_EXP_ECALL_U\r\n");
            break;

        case CODE_MCAUSE_EXP_ECALL_S:
            uart_puts(uart0, "CODE_MCAUSE_EXP_ECALL_S\r\n");
            break;

        case CODE_MCAUSE_EXP_ECALL_M:
            uart_puts(uart0, "CODE_MCAUSE_EXP_ECALL_M\r\n");
            break;

        case CODE_MCAUSE_EXP_INST_PAGE_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_INST_PAGE_FAULT\r\n");
            break;

        case CODE_MCAUSE_EXP_LOAD_PAGE_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_LOAD_PAGE_FAULT\r\n");
            break;

        case CODE_MCAUSE_EXP_STORE_PAGE_FAULT:
            uart_puts(uart0, "CODE_MCAUSE_EXP_STORE_PAGE_FAULT\r\n");
            break;

        default:
            uart_puts(uart0, "UNKNOWN/RESERVED\r\n");
            break;
        }
    }

    uart_puts(uart0, "==================\r\n");
    spin2unlock(&uart_lock);
}
