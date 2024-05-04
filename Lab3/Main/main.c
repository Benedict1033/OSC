#include "../header/mini_uart.h"
#include "../header/shell.h"
#include "../header/devicetree.h"

extern void *_dtb_ptr;


void print_current_el(void)
{
    unsigned long current_el;
    asm volatile("mrs %0, CurrentEL" : "=r" (current_el));
    uart_send_string("==== CurrentEL: ");
    uart_hex((current_el >> 2) & 0x3);
    uart_send_string("\n");
}


void kernel_main(void)
{
    uart_send_string("Hello, world!\n");


    fdt_traverse(get_initramfs_addr, _dtb_ptr);
    
 print_current_el(); // r

    shell();
}