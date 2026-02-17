ENTRY(__entry) /* shut up linker */

SECTIONS {

    __uart0 = 0x10010000;
    __uart1 = 0x10011000;

    . = 0x80000000;
    .text :{
        out/entry.o(.text);
        *(.text .text.*);
    }

    .rodata : ALIGN(8) {
        *(.rodata .rodata.*);
    }

    .data : ALIGN(8) {
        *(.data .data.*);
    }

    .bss : ALIGN(8) {
        /* we will need to 0 this */
        __bss = .;
        *(.bss .bss.* .sbss .sbss.*);
        __bss_end = .;
    }

    . = ALIGN(8);
    __ready = .;
    . += 8; /* address size */
    . += 8; /* scratch */

    . = ALIGN(8);
    . += 10 * 1024 * 1024; /* 10MiB */
    __stack_base = .;
}
