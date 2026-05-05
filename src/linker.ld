ENTRY(_) /* Useless for our case but makes linker shut up. */

SECTIONS {
    clintctrl   = 0x2000000;
    clckctrl    = 0x10000000;
    uart0       = 0x10010000;
    uart1       = 0x10011000;

    . = 0x80000000;
    .text :{
        /*
            Linker's ENTRY sets it in bin container but we dont keep that.
            The true <<entry>> must be placed exactly at 0x80000000.
        */
        *(.text._uunix_entry);
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
