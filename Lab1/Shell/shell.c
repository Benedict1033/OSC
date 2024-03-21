#include "header/shell.h"
#include "../header/mini_uart.h"
#include "../header/utils.h"
#include "../header/mailbox.h"
#include "../header/reboot.h"
#include <stddef.h>

#define BUFFER_MAX_SIZE 256u
#define COMMNAD_LENGTH_MAX 20u

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
    uart_send_string("help     :");
    uart_send_string("print this help menu\r\n");
    uart_send_string("hello    :");
    uart_send_string("print Hello World!\r\n");
    uart_send_string("reboot   :");
    uart_send_string("reboot the device\r\n");
    uart_send_string("info     :");
    uart_send_string("the mailbox hardware info\r\n");
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
    utils_newline2end(buffer);
    uart_send('\r');

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
        uart_send_string("rebooting...\r\n");
        reset(1000);
    }
    else if (utils_str_compare(buffer, "info") == 0)
    {
        info();
    }
    else
    {
        uart_send_string("commnad '");
        uart_send_string(buffer);
        uart_send_string("' not found\r\n");
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
