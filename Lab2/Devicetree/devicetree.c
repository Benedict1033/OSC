#include "../header/devicetree.h"
#include "../header/cpio.h"
#include "../header/mini_uart.h"
#include "../header/utils.h"

int space = 0;
char *cpio_addr;

// Function to send a byte in hexadecimal format
void uart_send_hex(unsigned int val) {
    const char hex_digits[] = "0123456789ABCDEF";
    for (int i = 28; i >= 0; i -= 4) {
        uart_send(hex_digits[(val >> i) & 0xF]);
    }
}

// Custom function for string length
size_t custom_strlen(const char *str) {
    const char *s = str;
    while (*s) ++s;
    return s - str;
}

// Custom string compare function
int custom_strcmp(const char *str1, const char *str2) {
    while (*str1 && (*str1 == *str2)) {
        ++str1;
        ++str2;
    }
    return *(unsigned char *)str1 - *(unsigned char *)str2;
}

// Convert 4 bytes from big-endian to host endian format
uint32_t get_be32(const void *ptr) {
    const uint8_t *bytes = (const uint8_t *)ptr;
    return ((uint32_t)bytes[0] << 24) | ((uint32_t)bytes[1] << 16) | ((uint32_t)bytes[2] << 8) | bytes[3];
}

int parse_struct(fdt_callback cb, uintptr_t cur_ptr, uintptr_t strings_ptr, uint32_t totalsize) {
    uintptr_t end_ptr = cur_ptr + totalsize;
    while (cur_ptr < end_ptr) {
        uint32_t token = get_be32((const void *)cur_ptr);
        cur_ptr += 4;
        switch (token) {
            case FDT_BEGIN_NODE:
                cb(token, (char *)cur_ptr, NULL, 0);
                cur_ptr += align_up(custom_strlen((char *)cur_ptr) + 1, 4);
                break;
            case FDT_END_NODE:
                cb(token, NULL, NULL, 0);
                break;
            case FDT_PROP: {
                uint32_t len = get_be32((const void *)cur_ptr);
                cur_ptr += 4;
                uint32_t nameoff = get_be32((const void *)cur_ptr);
                cur_ptr += 4;
                cb(token, (char *)(strings_ptr + nameoff), (void *)cur_ptr, len);
                cur_ptr += align_up(len, 4);
                break;
            }
            case FDT_NOP:
                cb(token, NULL, NULL, 0);
                break;
            case FDT_END:
                cb(token, NULL, NULL, 0);
                return 0;
        }
    }
    return -1; // Reached end without finding FDT_END
}

void get_initramfs_addr(int type, const char *name, const void *data, uint32_t size) {
    print_dtb(type, name, data, size);
    if (type == FDT_PROP && custom_strcmp(name, "linux,initrd-start") == 0) {
        cpio_addr = (char *)(uintptr_t)get_be32(data);
        uart_send_string("Initramfs start address: ");
        uart_send_hex((unsigned long)cpio_addr);
        uart_send_string("\n");
    }
}

int fdt_traverse(fdt_callback cb, void* _dtb) {
    struct fdt_header *header = _dtb;
    if (get_be32(&header->magic) != 0xd00dfeed) {
        uart_send_string("Invalid FDT magic number.\n");
        return -1;
    }
    uintptr_t struct_ptr = (uintptr_t)_dtb + get_be32(&header->off_dt_struct);
    uintptr_t strings_ptr = (uintptr_t)_dtb + get_be32(&header->off_dt_strings);
    uint32_t totalsize = get_be32(&header->totalsize);
    return parse_struct(cb, struct_ptr, strings_ptr, totalsize);
}

void print_property_data(const char *name, const void *data, uint32_t size) {
    if (custom_strcmp(name, "compatible") == 0 || custom_strcmp(name, "status") == 0) {
        // Treat as a null-terminated string
        uart_send_string("\"");
        uart_send_string((const char *)data);
        uart_send_string("\"");
    } else {
        // Assume it's a series of 32-bit integers
        const uint32_t *words = (const uint32_t *)data;
        for (uint32_t i = 0; i < size / 4; i++) {
            if (i > 0) uart_send_string(", ");
            uart_send_hex(words[i]);
        }
    }
}

// Modify print_dtb function
void print_dtb(int type, const char *name, const void *data, uint32_t size) {
    switch (type) {
        case FDT_BEGIN_NODE:
            send_space(space * 2);
            uart_send_string(name ? name : "");
            uart_send_string(" {\n");
            space++;
            break;
        case FDT_END_NODE:
            space--;
            send_space(space * 2);
            uart_send_string("};\n");
            break;
        case FDT_PROP:
            send_space(space * 2);
            uart_send_string(name);
            uart_send_string(" = ");
            print_property_data(name, data, size); // Print property data
            uart_send_string(";\n");
            break;
    }
}