#include "../header/mini_uart.h"
#include "../header/utils.h"

void delay(unsigned int clock)
{
    while (clock--)
    {
        asm volatile("nop");
    }
}

void uart_init()
{
    unsigned int selector;

    selector = *GPFSEL1;
    selector &= ~(7 << 12); // clean gpio14
    selector |= 2 << 12;    // set alt5 for gpio14
    selector &= ~(7 << 15); // clean gpio15
    selector |= 2 << 15;    // set alt5 for gpio 15
    *GPFSEL1 = selector;

    *GPPUD = 0;  // set the required control signal (i.e. Pull-up or Pull-Down )
    delay(150); // provides the required set-up time for the control signal
    *GPPUDCLK0 = (1 << 14) | (1 << 15);
    delay(150);
    *GPPUDCLK0 = 0;

    *AUX_ENABLE = 1;        // Enable mini uart (this also enables access to its registers)
    *AUX_MU_CNTL_REG = 0;   // Disable auto flow control and disable receiver and transmitter (for now)
    *AUX_MU_IER_REG = 0;    // Disable receive and transmit interrupts
    *AUX_MU_LCR_REG = 3;    // Enable 8 bit mode
    *AUX_MU_MCR_REG = 0;    // Set RTS line to be always high
    *AUX_MU_BAUD_REG = 270; // Set baud rate to 115200
    *AUX_MU_IIR_REG = 6;
    *AUX_MU_CNTL_REG = 3; // Finally, enable transmitter and receiver
}

char uart_recv()
{
    while (!(*(AUX_MU_LSR_REG)&0x01)){}

    char temp = *(AUX_MU_IO_REG);
    return temp == '\r' ? '\n' : temp;
}

void uart_send(const char c)
{
    if (c == '\n')
        uart_send('\r');

    while (!(*(AUX_MU_LSR_REG)&0x20)){}

    *AUX_MU_IO_REG = c;
}


void uart_send_string(const char *str)
{
    while (*str)
    {
        uart_send(*str++);
    }
}

void uart_hex(unsigned int d)
{
    unsigned int n;
    int c;
    uart_send_string("0x");
    for (c = 28; c >= 0; c -= 4)
    {
        n = (d >> c) & 0xF;
        n += n > 9 ? 0x57 : 0x30;
        uart_send(n);
    }
}