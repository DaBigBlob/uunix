.extern main

.section .bss
.globl kheap_top
.globl kstack_base
kheap_top:
    .space 1024 * 1024 /* 1MiB */
kstack_base:

.section .text
.globl _start
_start:
    csrr a0, mhartid
    la   sp, stack_base
    slli t1, a0, 12 /* 2^12 = 4096 bytes = 4KiB for each hart (max 256 harts)*/
    sub  sp, sp, t1

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
