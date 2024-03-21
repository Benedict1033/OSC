#include "header/bootloader.h"
#include "../header/mini_uart.h"
#include "../header/utils.h"

#include <stddef.h>
#define BUFFER_MAX_SIZE 256u

void load_img()
{
    char *const kernel_addr = (char *)0x80000;
    uart_send_string("Please sent the kernel image size:\n");
    char buffer[BUFFER_MAX_SIZE];
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
    buffer[index] = '\0'; // Fix for proper string termination.
    utils_newline2end(buffer);
    uart_send_string("\r\n");
    unsigned int img_size = utils_str2uint_dec(buffer); // Ensure this function is properly declared and defined.
    uart_send_string("Start to load the kernel image...\n");

    unsigned char *current = (unsigned char *)kernel_addr; // Correct type for pointer arithmetic.
    unsigned int original_img_size = img_size; // Save original size for progress calculation.
    while (img_size--)
    {
        *current++ = uart_recv(); // Proper increment of pointer.
        if ((original_img_size - img_size) % 512 == 0) { // Print a dot every 512 bytes received.
            uart_send('.');
        }
    }
    uart_send_string("\nKernel loaded successfully.\n");
    // branchAddr(kernel_addr); // Ensure this function exists and works as expected.
    asm volatile("mov sp, %0; br %0" :: "r" (kernel_addr) : "memory"); // Update if necessary for your setup.
}
