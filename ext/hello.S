.section .text
.globl _start
.balign 4

.equ SYS_WRITE, 2
.equ SYS_EXIT,  3

_start:
    la  t0, msg

wrt:
    lbu     a0, 0(t0)
    beqz    a0, done

    li      a7, SYS_WRITE
    ecall

    addi    t0, t0, 1
    j       wrt

done:

    .balign 4
msg:
    .asciz "hello hell\r\n"
