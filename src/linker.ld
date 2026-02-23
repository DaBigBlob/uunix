SECTIONS {

    uart0 = 0x10010000;
    uart1 = 0x10011000;

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
        bss_begin = .;
        *(.bss .bss.* .sbss .sbss.*);
        bss_end = .;
    }
}
