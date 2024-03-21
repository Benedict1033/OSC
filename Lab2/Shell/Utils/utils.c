#include "header/utils.h"
#include <stddef.h>

int utils_str_compare(const char *a, char *b)
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

size_t utils_strlen(const char *s) {
  size_t i = 0;
  while (s[i]) i++;
  return i+1;
}

uint32_t align_up(uint32_t size, int alignment) {
  return (size + alignment - 1) & -alignment;
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

void utils_int2str_dec(int num, char *str)
{
    // num=7114 digit=4
    int digit = -1, temp = num;
    while (temp > 0)
    {
        temp /= 10;
        digit++;
    }
    for (int i = digit; i >= 0; i--)
    {
        int t = 1;
        for (int j = 0; j < i; j++)
        {
            t *= 10;
        }
        *str = '0' + num / t;
        num = num % t;
        str++;
    }
    *str = '\0';
}

void utils_uint2str_dec(unsigned int num, char *str)
{
    // num=7114 digit=4
    unsigned int temp = num;
    int digit = -1;
    while (temp > 0)
    {
        temp /= 10;
        digit++;
    }
    for (int i = digit; i >= 0; i--)
    {
        int t = 1;
        for (int j = 0; j < i; j++)
        {
            t *= 10;
        }
        *str = '0' + num / t;
        num = num % t;
        str++;
    }
    *str = '\0';
}
void utils_uint2str_hex(unsigned int num, char *str)
{
    // num=7114 digit=4
    unsigned int temp = num;
    int digit = -1;
    *str = '0';

    *str = 'x';

    if (num == 0)
    {
        *str = '0';
        str++;
    }
    else
    {
        while (temp > 0)
        {
            temp /= 16;
            digit++;
        }
        for (int i = digit; i >= 0; i--)
        {
            int t = 1;
            for (int j = 0; j < i; j++)
            {
                t *= 16;
            }
            if (num / t >= 10)
            {
                *str = '0' + num / t + 39;
            }
            else
            {
                *str = '0' + num / t;
            }
            num = num % t;
            str++;
        }
    }
    *str = '\0';
}