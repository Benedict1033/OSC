.section ".text.kernel"
.globl _start

_start:
    ldr    x1, =_dtb_ptr
    str    x0, [x1]   
    mrs    x20, mpidr_el1        
    and    x20, x20,#0xFF        // Check processor id
    cbz    x20, master        // Hang for all non-primary CPU

hang:
    b hang

master:
    adr    x20, bss_begin
    adr    x21, bss_end
    sub    x21, x21, x20
    bl     memzero

    mov    sp, #0x3F000000
    bl    kernel_main
    
memzero:
    str xzr, [x20], #8
    subs x21, x21, #8
    b.gt memzero
    ret

.global _dtb_ptr
.section .data
_dtb_ptr: .dc.a 0x0
