#include "trap.h"
#include "base.h"
#include "lock.h"
#include "mem.h"
#include "pre.h"
#include "std.h"
#include "uart.h"
#include "task.h"

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

#define UNHANDLED_TRAP_LIST_INTR(x, s) x(INTR_TIMER) s x(INTR_EXTERNAL)
#define UNHANDLED_TRAP_LIST_EXP(x, s)                                     \
    s x(EXP_INST_ADDR_MISALIGNED)                                         \
    s x(EXP_INST_ACCESS_FAULT)                                            \
    s x(EXP_ILLEGAL_INST)                                                 \
    s x(EXP_BREAKPOINT)                                                   \
    s x(EXP_LOAD_ADDR_MISALIGNED)                                         \
    s x(EXP_LOAD_ACCESS_FAULT)                                            \
    s x(EXP_STORE_ADDR_MISALIGNED)                                        \
    s x(EXP_STORE_ACCESS_FAULT)                                           \
    s x(EXP_ECALL_S)                                                      \
    s x(EXP_ECALL_M)                                                      \
    s x(EXP_INST_PAGE_FAULT)                                              \
    s x(EXP_LOAD_PAGE_FAULT)                                              \
    s x(EXP_STORE_PAGE_FAULT)
#define UNHANDLED_TRAP_LIST(x, s)                                         \
    UNHANDLED_TRAP_LIST_INTR(x, s) s UNHANDLED_TRAP_LIST_EXP(x, s)

void trap_handle(void)
{
    // disable int: trap depth 1
    // no need to reenable int (we restore mstatus from HCB)
    set_mstatus(get_mstatus() & ~MASK_MSTATUS_MIE);

    HCB        *hcb    = compute_hartid2HCB(get_mhartid());
    const usize mcause = (usize)hcb->mcause, mepc = (usize)hcb->mepc,
                mtval = (usize)hcb->mtval, mstatus = (usize)hcb->mstatus;

    /*======trap=cases=begin=============================================*/
    /* syscalls should be lean-and-mean **********************************/
    if (!MCAUSE_IS_INTR(mcause) &&
        MCAUSE_CODE(mcause) == CODE_MCAUSE_EXP_ECALL_U) {
        ///////// placeholder code begin///////////////////////////////////
        spin2lock(&uart0_lock);
        uart_puts(uart0, "\r\n======TRAP:EXP_ECALL_U\r\n");

        /* increment pc beyond ecall */
        hcb->mepc = (any)((usize)hcb->mepc + 4); // ecall is 32bit

        uart_puts(uart0, "\r\nsyscall args:");
#define df0(a)                                                            \
    uart_puts(uart0, "\r\n    " #a ":");                                  \
    uart_putu64(&uart0, (u64)hcb->a)
        REGISTER_LIST_a(df0, ;);
#undef df0
        spin2unlock(&uart0_lock);
        ///////// placeholder code end/////////////////////////////////////
        goto trap_handle_ret;
    }

    /* from here on we provide verbose-ish trap info */
    spin2lock(&uart0_lock);

    /* hart tasks ********************************************************/
    if (MCAUSE_IS_INTR(mcause) &&
        MCAUSE_CODE(mcause) == CODE_MCAUSE_INTR_SOFT) {
        /* say stuff before sending to task */
        uart_puts(uart0, "\r\n======TRAP:INTR_SOFT(HART_TASK)\r\n");

        /* clear MSIP before ret or mret immediately traps again */
        unset_msip(hcb->hartid);

        uart_puts(uart0, "exec-ing code at:\r\n    ");
        uart_putu64(&uart0, (u64)hcb->cmd.func);
        uart_puts(uart0, "\r\nwith privilege mode:\r\n    ");
        uart_putu64(&uart0, (u64)hcb->cmd.mpp);
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

        /* set privilege */
        // clear mpp
        hcb->mstatus = (any)((usize)(hcb->mstatus) & ~MASK_MSTATUS_MPP);
        hcb->mstatus = (any)((usize)(hcb->mstatus) | hcb->cmd.mpp);

        /* task at hcb->cmd.func should spin2unlock(&hcb->cmd.lock)
            this after itself*/
        goto trap_handle_info;
    }

    /* deal with unhandled traps *****************************************/
    /* TODO: save trap state somewhere (HCB?) so hart0 can take a look */
    if (MCAUSE_IS_INTR(mcause)) {
        switch (MCAUSE_CODE((usize)hcb->mcause)) {
#define df0(a)                                                            \
    case CODE_MCAUSE_##a:                                                 \
        uart_puts(uart0, "\r\n======TRAP:" STR(a) "\r\n");                \
        hcb->mepc    = (any)task_done; /* wait for new instructions */    \
        hcb->sp      = compute_hartid2HCB(hcb->hartid); /* reuse*/        \
        hcb->mstatus = (any)(((usize)hcb->mstatus & ~MASK_MSTATUS_MPP) |  \
                             CODE_MSTATUS_MPP_M |                         \
                             MASK_MSTATUS_MPIE); /* make sure M mode */   \
        break;

            UNHANDLED_TRAP_LIST_INTR(df0, )
        default:
            break;
        }
    } else {
        switch (MCAUSE_CODE((usize)hcb->mcause)) {
            UNHANDLED_TRAP_LIST_EXP(df0, )
#undef df0
        default:
            break;
        }
    }
    /*======trap=cases=end===============================================*/

trap_handle_info:
    uart_puts(uart0, "---\r\n");
#define jjshow(thing)                                                     \
    uart_puts(uart0, #thing "\t= ");                                      \
    uart_putu64(&uart0, thing);                                           \
    uart_puts(uart0, "\r\n")

    uart_puts(uart0, "hartid\t= ");
    uart_putu64(&uart0, (u64)hcb->hartid);
    uart_puts(uart0, "\r\n");

    jjshow(mcause);

    uart_puts(uart0, "  code\t= ");
    uart_putu64(&uart0, (u64)MCAUSE_CODE((usize)hcb->mcause));
    uart_puts(uart0, "\r\n");

    jjshow(mepc);
    jjshow(mtval);
    jjshow(mstatus);

    uart_puts(uart0, "mie\t= ");
    uart_putu64(&uart0, (u64)get_mie());
    uart_puts(uart0, "\r\n");
#undef jjshow
    uart_puts(uart0, "==================\r\n");
    spin2unlock(&uart0_lock);

trap_handle_ret:
    return;
}
