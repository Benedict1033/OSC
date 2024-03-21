.section ".text.boot"
.global _start

_start:
    mrs x1, mpidr_el1
    and x1, x1, #3
    cbz x1, setting
    b halt

setting: 
	ldr x1, =_start
 	mov sp, x1

  ldr x1, =__bss_start
    ldr x2, =__bss_end
    sub x2, x2, x1 

clear_bss:
    cbz x2, kernel_main
    str xzr, [x1], #8
    sub x2, x2, #8
    b clear_bss

kernel_main: 
    bl main
    b halt

halt: 
    wfe
    b halt