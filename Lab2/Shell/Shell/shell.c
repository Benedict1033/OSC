#include "header/shell.h"
#include "../header/mini_uart.h"
#include "../header/utils.h"
#include "../header/mailbox.h"
#include "../header/reboot.h"
#include "../header/devicetree.h"
#include "../header/allocator.h"
#include "../header/cpio.h"

#include <stddef.h>
#define BUFFER_MAX_SIZE 256u
#define COMMNAD_LENGTH_MAX 20u

extern void *_dtb_ptr;

void read_command(char *buffer)
{
    size_t index = 0;
    char ch;
    while (1)
    {
        ch = uart_recv();
        // Ignore non-printable characters and ASCII control characters
        if (ch >= 32 || ch == '\n')
        {
            buffer[index++] = ch;
            uart_send(ch); // Echo back if it's a printable character
        }

        if (ch == '\n')
        {
            break;
        }
    }
    buffer[index] = '\0'; // Properly terminate the string
}

void help()
{
    uart_send_string("help    :");
    uart_send_string("print this help menu\n");
    uart_send_string("hello   :");
    uart_send_string("print Hello World!\n");
    uart_send_string("reboot  : ");
    uart_send_string("reboot the device\n");
    uart_send_string("info    : ");
    uart_send_string("the mailbox hardware info\n");
    uart_send_string("ls      : ");
    uart_send_string("list the all file\n");
    uart_send_string("cat     : ");
    uart_send_string("print the file content\n");
    uart_send_string("malloc  : ");
    uart_send_string("a simple memory allocator\n");
    uart_send_string("dtb     : ");
    uart_send_string("print the device name tree \n");
}

void hello()
{
    uart_send_string("Hello World!\n");
}

void info()
{
    get_board_revision();
    get_arm_memory();
}

void parse_command(char *buffer)
{

    if (buffer[0] == '\0')
    { // enter empty
        return;
    }
    else if (utils_str_compare(buffer, "help") == 0)
    {
        help();
    }
    else if (utils_str_compare(buffer, "hello") == 0)
    {
        hello();
    }
    else if (utils_str_compare(buffer, "reboot") == 0)
    {
        uart_send_string("rebooting...\n");
        reset(1000);
    }
    else if (utils_str_compare(buffer, "info") == 0)
    {
        info();
    }
    else if (utils_str_compare(buffer, "ls") == 0)
    {
        cpio_ls();
    }
    else if (utils_str_compare(buffer, "cat") == 0)
    {
        uart_send_string("Filename: ");
        char buffer[BUFFER_MAX_SIZE];
        read_command(buffer);
        cpio_cat(buffer);
    }
    else if (utils_str_compare(buffer, "malloc") == 0)
    {
        char *a = malloc(sizeof("1234"));
        char *b = malloc(sizeof("789"));
        a[0] = '0';
        a[1] = '1';
        a[2] = '2';
        a[3] = '3';
        a[4] = '\0';
        b[0] = '7';
        b[1] = '8';
        b[2] = '9';
        b[3] = '\0';
        // uart_hex((unsigned int)a);
        // uart_send('\n');
        uart_send_string(a);
        uart_send('\n');
        // uart_hex((unsigned int)b);
        // uart_send('\n');
        uart_send_string(b);
        uart_send('\n');
    }
    else if (utils_str_compare(buffer, "dtb") == 0)
    {
        // fdt_traverse(print_dtb,_dtb_ptr);
    }
    else
    {
        uart_send_string("commnad '");
        uart_send_string(buffer);
        uart_send_string("' not found\n");
    }
}

void shell()
{
    while (1)
    {
        char buffer[BUFFER_MAX_SIZE];
        uart_send_string("$ ");
        read_command(buffer);
        parse_command(buffer);
    }
}