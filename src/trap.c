#include "trap.h"
#include "base.h"
#include "lock.h"
#include "mem.h"
#include "std.h"

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

    uart_puts(uart0, "code    = ");
    uart_putu64(&uart0, (u64)(hcb->mcause & ~MASK_MCAUSE_INTR));
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

    if (hcb->mcause & MASK_MCAUSE_INTR) {
        uart_puts(uart0, "INT: ");

        switch (hcb->mcause & ~MASK_MCAUSE_INTR) {
        case 3:
            uart_puts(uart0, "Software\r\n");
            /* clear MSIP before ret or mret immediately traps again */
            unset_msip(hcb->hartid);
            break;

        case 7:
            uart_puts(uart0, "Timer\r\n");
            break;

        case 11:
            uart_puts(uart0, "PLIC/External\r\n");
            break;

        default:
            uart_puts(uart0, "Unknown/Reserved\r\n");
            break;
        }
    } else {
        uart_puts(uart0, "EXP: ");

        switch ((hcb->mcause & ~MASK_MCAUSE_INTR)) {
        case 0:
            uart_puts(uart0, "Instruction address misaligned\r\n");
            break;

        case 1:
            uart_puts(uart0, "Instruction access fault\r\n");
            break;

        case 2:
            uart_puts(uart0, "Illegal instruction\r\n");
            break;

        case 3:
            uart_puts(uart0, "Breakpoint\r\n");
            break;

        case 4:
            uart_puts(uart0, "Load address misaligned\r\n");
            break;

        case 5:
            uart_puts(uart0, "Load access fault\r\n");
            break;

        case 6:
            uart_puts(uart0, "Store/AMO address misaligned\r\n");
            break;

        case 7:
            uart_puts(uart0, "Store/AMO access fault\r\n");
            break;

        case 8:
            uart_puts(uart0, "Ecall from U-mode\r\n");
            break;

        case 9:
            uart_puts(uart0, "Ecall from S-mode\r\n");
            break;

        case 10:
            uart_puts(uart0, "Reserved exception 10\r\n");
            break;

        case 11:
            uart_puts(uart0, "Ecall from M-mode\r\n");
            break;

        case 12:
            uart_puts(uart0, "Instruction page fault\r\n");
            break;

        case 13:
            uart_puts(uart0, "Load page fault\r\n");
            break;

        case 14:
            uart_puts(uart0, "Reserved exception 14\r\n");
            break;

        case 15:
            uart_puts(uart0, "Store/AMO page fault\r\n");
            break;

        default:
            uart_puts(uart0, "Unknown/Reserved exception\r\n");
            break;
        }
    }

    uart_puts(uart0, "==================\r\n");
    spin2unlock(&uart_lock);
}
