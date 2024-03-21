#include "UART/header/mini_uart.h"
#include "Bootloader/header/bootloader.h"

void main(void)
{
    uart_init();
    load_img();
}
