#include "header/shell.h"
#include "../header/utils.h"
#include "../header/mini_uart.h"
#include "../header/mailbox.h"
#include "../header/cpio.h"
#include "../header/allocator.h"
#include "../header/devicetree.h"
#include "../header/reboot.h"
#include <stddef.h>

#define BUFFER_MAX_SIZE 256
#define COMMNAD_LENGTH_MAX 20

extern void *_dtb_ptr;

void read_command(char *buffer)
{
    size_t index = 0;
    while (1)
    {
        buffer[index] = uart_recv();
        uart_send(buffer[index]);
        if (buffer[index] == '\n')
        {
            break;
        }
        index++;
    }
    buffer[index + 1] = '\0';
    utils_newline2end(buffer);
    uart_send('\r');
}

void help()
{
    uart_send_string("help    : print this help menu\n");
    uart_send_string("hello   : print Hello World!\n");
    uart_send_string("reboot  : reboot the device\n");
    uart_send_string("info    : the mailbox hardware info\n");
    uart_send_string("ls      : list the all file\n");
    uart_send_string("cat     : print the file content\n");
    uart_send_string("malloc  : a simple memory allocator\n");
    uart_send_string("dtb     : print the device name tree \n");
}

void reboot()
{
    uart_send_string("rebooting...\r\n");
    reset(1000);
}

void hello()
{
    uart_send_string("Hello World!\r\n");
}

void info()
{
    get_board_revision();
    get_arm_memory();
}

void parse_command(char *buffer)
{
    if (buffer[0] == '\0')
        return;
    else if (utils_str_compare(buffer, "help") == 0)
        help();
    else if (utils_str_compare(buffer, "hello") == 0)
        hello();
    else if (utils_str_compare(buffer, "reboot") == 0)
        reboot();
    else if (utils_str_compare(buffer, "info") == 0)
        info();
    else if (utils_str_compare(buffer, "ls") == 0)
        cpio_ls();
    else if (utils_str_compare(buffer, "cat") == 0)
    {
        uart_send_string("Filename: ");
        char buffer[BUFFER_MAX_SIZE];
        read_command(buffer);
        cpio_cat(buffer);
    }
    else if (utils_str_compare(buffer, "malloc") == 0)
    {
        char *a = malloc(sizeof("Hello"));
        char *b = malloc(sizeof("World!"));

        a[0] = 'H', a[1] = 'e', a[2] = 'l', a[3] = 'l', a[4] = 'o', a[5] = '\0';
        b[0] = 'W', b[1] = 'o', b[2] = 'r', b[3] = 'l', b[4] = 'd'; b[5] = '!', b[6] = '\0';

        uart_send_string(a);
        uart_send(' ');
        uart_send_string(b);
        uart_send('\n');
    }
    else if (utils_str_compare(buffer, "dtb") == 0)
        fdt_traverse(print_dtb,_dtb_ptr);
    else
        uart_send_string("commnad not found\r\n");
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
