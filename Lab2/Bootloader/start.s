.section ".text.relocate"
.globl _start_relocate

_start_relocate:
    adr x10, .               
    ldr x12, =text_begin  
    adr x13, bss_end
    sub x11, x13, x12

moving_relocate:
    cmp x10, x13            
    b.eq end_relocate          
    ldr x14, [x10], #8      
    str x14, [x12], #8      
    b moving_relocate          

end_relocate:
    ldr x14, =boot_entry    
    br x14

//------------------------------

.section ".text.boot"
.globl _start_boot

_start_boot:
    ldr x20, =_dtb
    str x0, [x20]
    mrs    x20, mpidr_el1        
    and    x20, x20,#0xFF 
    cbz    x20, master  

hang:
    b hang

master:
    adr    x20, bss_begin
    adr    x21, bss_end
    sub    x21, x21, x20
    bl     memzero

    mov    sp, #0x3F000000
    bl    bootloader_main

memzero:
	str xzr, [x20], #8
	subs x21, x21, #8
	b.gt memzero
	ret
    
.global _dtb
.section .data
_dtb: .dc.a 0x0
