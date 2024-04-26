SECTIONS
{
  . = 0x60000;
  
  text_begin = .; 
  .text.relo : { *(.text.relocate) }

  boot_entry = .; 
  .text.boot : { *(.text.boot) }

  .text :  { *(.text) }

  .rodata : {  *(.rodata) }

  .data : { *(.data) }

  bss_begin = .; 
  .bss (NOLOAD) : { *(.bss) }
  . = ALIGN(8); 
  bss_end = .;  
}