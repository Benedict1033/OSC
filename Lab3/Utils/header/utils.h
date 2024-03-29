#include <stddef.h>
#include <stdint.h>

int utils_str_compare(const char *a,const char *b);
void utils_newline2end(char *str);
unsigned int utils_str2uint_dec(const char *str);
unsigned long hex2dec(char *s);
void align(void *size, size_t s); 
size_t utils_strlen(const char *s);
uint32_t get_le2be_uint(const void *p);
void send_sapce(int n);
void uart_hex(unsigned int d);
uint32_t align_up(uint32_t size, int alignment);