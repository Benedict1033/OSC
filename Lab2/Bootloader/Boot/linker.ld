SECTIONS
{
    . = 0x60000;

    .text : {
        KEEP(*(.text.boot))
        *(.text)
    }

    .rodata : { *(.rodata) }

    .data : { *(.data) }

    .bss (NOLOAD) : {
        . = ALIGN(16);
        __bss_start = .;  
        *(.bss)
        __bss_end = .;    
    }
}
