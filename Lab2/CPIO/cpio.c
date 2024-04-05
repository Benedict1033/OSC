#include "header/cpio.h"
#include "../header/mini_uart.h"
#include "../header/utils.h"

void cpio_ls()
{
    char *addr = cpio_addr;

    while (utils_str_compare((char *)(addr + sizeof(cpio_header)), "TRAILER!!!") != 0)
    {
        cpio_header *header = (cpio_header *)addr;
        unsigned long pathname_size = hex2dec(header->c_namesize);
        unsigned long file_size = hex2dec(header->c_filesize);
        unsigned long headerPathname_size = sizeof(cpio_header) + pathname_size;

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

    return 0;
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
        for (unsigned int i = 0; i < file_size; i++)
        {
            uart_send(file_content[i]); // print the file content
        }
        uart_send_string("\n");
    }
    else
        uart_send_string("Not found the file\n");
}
