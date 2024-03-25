SECTIONS
{
    . = 0x80000;

    .text.boot : { *(.text.boot) }

    .text :  { *(.text) }
    
    .rodata : { *(.rodata) }
    
    .data : { *(.data) }

    . = ALIGN(8);
    bss_begin = .;  
    .bss (NOLOAD) : { *(.bss) }
    bss_end = .; 
}
