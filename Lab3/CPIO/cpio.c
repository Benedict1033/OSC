#include "header/cpio.h"
#include "../header/mini_uart.h"
#include "../header/utils.h"
#include "../header/allocator.h"

void cpio_ls()
{
    char *addr = cpio_addr;

    while (utils_str_compare((char *)(addr + sizeof(cpio_header)), "TRAILER!!!") != 0)
    {
        cpio_header *header = (cpio_header *)addr;
        unsigned long pathname_size = hex2dec(header->c_namesize); //path_size
        unsigned long file_size = hex2dec(header->c_filesize); //file_size

        unsigned long headerPathname_size = sizeof(cpio_header) + pathname_size; //total_size

        align(&headerPathname_size,4); 
        align(&file_size,4);           

        uart_send_string(addr + sizeof(cpio_header)); // print the file name
        uart_send_string("\n");

        addr += (headerPathname_size + file_size);
    }
}

char *findFile(char *name)
{
    char *addr = cpio_addr;
    while (utils_str_compare((char *)(addr + sizeof(cpio_header)), "TRAILER!!!") != 0)
    {
        if ((utils_str_compare((char *)(addr + sizeof(cpio_header)), name) == 0))
            return addr;
        
        cpio_header *header = (cpio_header *)addr;
        unsigned long pathname_size = hex2dec(header->c_namesize);
        unsigned long file_size = hex2dec(header->c_filesize);
        unsigned long headerPathname_size = sizeof(cpio_header) + pathname_size;

        align(&headerPathname_size,4); 
        align(&file_size,4);           
        addr += (headerPathname_size + file_size);
    }
}

void cpio_cat(char *filename)
{
    char *target = findFile(filename);

    if (target)
    {
        cpio_header *header = (cpio_header *)target;
        unsigned long pathname_size = hex2dec(header->c_namesize);
        unsigned long file_size = hex2dec(header->c_filesize);
        unsigned long headerPathname_size = sizeof(cpio_header) + pathname_size;

        align(&headerPathname_size,4); 
        align(&file_size,4);           

        char *file_content = target + headerPathname_size;

        uart_send_string(file_content);
        uart_send_string("\n");
    }
    else
        uart_send_string("Not found the file\n");
}

// Function to load a user program and switch to EL0
void run_user_program(char* filename) {
    char* target = findFile(filename);
    if (!target) {
        uart_send_string("File not found\n");
        return;
    }

    cpio_header* header = (cpio_header*) target;
    unsigned long pathname_size = hex2dec(header->c_namesize);
    unsigned long file_size = hex2dec(header->c_filesize);
    unsigned long headerPathname_size = sizeof(cpio_header) + pathname_size;
    align(&headerPathname_size, 4);

    char* file_content = target + headerPathname_size;
    char* load_addr = (char*) 0x20000; // Destination address to load program

    for (unsigned i = 0; i < file_size; i++) {
        load_addr[i] = file_content[i];
    }

    uart_send_string("Program loaded to memory location 0x20000\n");

    // Set up environment for user program
    unsigned long clear_spsr = 0x0;
asm volatile ("msr spsr_el1, %0" :: "r"(clear_spsr));
unsigned long spsr_el1 = 0x3c0;
asm volatile ("msr spsr_el1, %0" :: "r"(spsr_el1));

    unsigned long jump_addr = 0x20000;
    asm volatile("msr elr_el1, %0" :: "r"(jump_addr));
    unsigned long sp = (unsigned long) malloc(4096) + 4096;  // Allocate and point to top of the stack
    asm volatile("msr sp_el0, %0" :: "r"(sp));
    
    uart_send_string("Switching to user program...\n");
    asm volatile("eret");
}