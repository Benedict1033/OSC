#include "UART/header/mini_uart.h"
#include "Shell/header/shell.h"

void main(void)
{
    uart_init();
    uart_send_string("Type in `help` to get instruction menu!\r\n");
    shell();
}
