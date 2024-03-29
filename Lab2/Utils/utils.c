#include "header/utils.h"
#include "../header/mini_uart.h"
#include <stddef.h>
#include <stdint.h>

int utils_str_compare(const char *a,const char *b)
{
    char aa, bb;
    do
    {
        aa = (char)*a++;
        bb = (char)*b++;
        if (aa == '\0' || bb == '\0')
        {
            return aa - bb;
        }
    } while (aa == bb);
    return aa - bb;
}

void utils_newline2end(char *str)
{
    while (*str != '\0')
    {
        if (*str == '\n')
        {
            *str = '\0';
            return;
        }
        ++str;
    }
}

unsigned int utils_str2uint_dec(const char *str)
{
    unsigned int value = 0u;

    while (*str)
    {
        value = value * 10u + (*str - '0');
        ++str;
    }
    return value;
}

unsigned long hex2dec(char *s)
{
    unsigned long r = 0;
    for (int i = 0; i < 8; ++i)
    {
        if (s[i] >= '0' && s[i] <= '9')
            r = r * 16 + s[i] - '0';
        else
            r = r * 16 + s[i] - 'a' + 10;
    }
    
    return r;
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


uint32_t get_le2be_uint(const void *p)
{
    // transfer little endian to big endian
    const unsigned char *bytes = p;
    uint32_t res = bytes[3];
    res |= bytes[2] << 8;
    res |= bytes[1] << 16;
    res |= bytes[0] << 24;
    return res;
}

void send_space(int count) {
    for (int i = 0; i < count; i++) {
        uart_send(' ');
    }
}

size_t utils_strlen(const char *s) {
  size_t i = 0;
  while (s[i]) i++;
  return i+1;
}

void align(void *size, size_t s) // aligned to 4 byte
{
/*
    The pathname is followed by NUL bytes so that the total size of the fixed header plus pathname is a multiple of 4.
    Likewise, the file data is padded to a multiple of 4 bytes.
*/
    unsigned long *x = (unsigned long *)size;
    if ((*x) & (s-1))
    {
        (*x) += s - ((*x) & (s-1));
    }
}

uint32_t align_up(uint32_t size, int alignment) {
  return (size + alignment - 1) & -alignment;
}
