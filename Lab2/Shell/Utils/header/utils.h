#include <stddef.h>
#include <stdint.h>

/* string */
int utils_str_compare(const char* a, char* b);
void utils_newline2end(char *str);
char utils_int2char(int a);
void utils_int2str_dec(int a,char* str);
void utils_uint2str_dec(unsigned int num,char* str);
void utils_uint2str_hex(unsigned int num,char* str);
void align(void *size, size_t s);
uint32_t align_up(uint32_t size, int alignment);
size_t utils_strlen(const char *s);


