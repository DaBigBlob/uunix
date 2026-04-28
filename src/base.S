.extern main

.section .bss
.globl kheap_top
.globl kstack_base
kheap_top:
    .space 256 * 4096 /* 1MiB */
kstack_base:

.section .text
.globl _start
_start:
    /* set sp for each hart */
    la   sp, stack_base
    csrr a0, mhartid
    slli t1, a0, 12 /* 2^12 = 4096 bytes = 4KiB for each hart */
    sub  sp, sp, t1

    /* reserve HCB space */
    addi sp, sp, -64


    j   main
    /* fall through if returned */


.section .text
.globl dead_spin
dead_spin:
    j   dead_spin


.section .text
.globl get_hartid
get_hartid:
    csrr a0, mhartid
    ret
