#include "../header/devicetree.h"
#include "../header/mini_uart.h"
#include "../header/utils.h"

int space = 0;
char *cpio_addr;

int parse_struct(fdt_callback cb, uintptr_t cur_ptr, uintptr_t strings_ptr, uint32_t totalsize)
{
    uintptr_t end_ptr = cur_ptr + totalsize;

    while (cur_ptr < end_ptr)
    {
        uint32_t token = get_le2be_uint((char *)cur_ptr);
        cur_ptr += 4;
        switch (token)
        {
            case FDT_BEGIN_NODE:
                cb(token, (char *)cur_ptr, NULL, 0);
                cur_ptr += align_up(utils_strlen((char *)cur_ptr), 4);   
                break;
            case FDT_END_NODE:
                cb(token, NULL, NULL, 0);
                break;
            case FDT_PROP:{
                uint32_t len = get_le2be_uint((char *)cur_ptr);
                cur_ptr += 4;
                uint32_t nameoff = get_le2be_uint((char *)cur_ptr);
                cur_ptr += 4;
                cb(token, (char *)(strings_ptr + nameoff), (void *)cur_ptr, len);
                cur_ptr +=  align_up(len, 4);;
                break;
            }
            case FDT_NOP:
                cb(token, NULL, NULL, 0);
                break;
            case FDT_END:
                cb(token, NULL, NULL, 0);
                return 0;
            default:;
                return -1;
        }
    }

     return -1;
}

void print_string(const char* data, uint32_t size) {
    uart_send_string("\"");
    for (uint32_t i = 0; i < size; i++) {
        if (data[i] == '\0' && i < size - 1) uart_send_string("\", \""); // For concatenated strings
        else uart_send(data[i]);
    }
    uart_send_string("\"");
}

void print_uint32_array_as_host_endian(const void* data, uint32_t size) {
    uart_send_string("<");
    for (uint32_t i = 0; i < size; i += 4) {
        if (i > 0) uart_send_string(", ");
        uint32_t val = get_be32((const uint8_t*)data + i);
        uart_hex(val);
    }
    uart_send_string(">");
}


void uart_send_int(int num )
{
    char str[256];
    utils_int2str_dec(num, str);
    uart_send_string(str);

}

void print_single_uint32_as_host_endian(const void* data) {
    uint32_t val = get_be32(data);
    uart_send_string("<");
    uart_send_int(val);
    uart_send_string(">");
}

void print_dtb(int type, const char* name, const void* data, uint32_t size) {
   switch (type) {
        case FDT_BEGIN_NODE:
            send_space(space * 2);
            uart_send_string(name);
            uart_send_string(" {\n");
            space++;
            break;
        case FDT_END_NODE:
            space--;
            send_space(space * 2);
            uart_send_string("}\n");
            break;
         case FDT_PROP:
            send_space(space * 2);
            uart_send_string(name);
            uart_send_string(" = ");
            if (utils_str_compare(name, "compatible") == 0 || utils_str_compare(name, "status") == 0 || utils_str_compare(name, "model") == 0 || utils_str_compare(name, "device_type") == 0) {
                print_string(data, size);
            } else if (utils_str_compare(name, "#address-cells") == 0 || utils_str_compare(name, "#size-cells") == 0 ) {
                print_single_uint32_as_host_endian(data); 
            } else {
                print_uint32_array_as_host_endian(data, size);
            }
            uart_send_string(";\n");
            break;

        case FDT_NOP:
        case FDT_END:
            break;
    }
}

void get_initramfs_addr(int type, const char *name, const void *data, uint32_t size)
{
    (void)size;
    if(type==FDT_PROP&&!utils_str_compare(name,"linux,initrd-start")){
        cpio_addr=(char *)(uintptr_t)get_le2be_uint(data);
        uart_send_string("initramfs_addr at ");
        uart_hex((uintptr_t)get_le2be_uint(data));
        uart_send('\n');
    }
}

int fdt_traverse(fdt_callback cb, void* _dtb)
{
    uintptr_t dtb_ptr = (uintptr_t)_dtb;
    uart_send_string("\ndtb loading at:");
    uart_hex(dtb_ptr);
    uart_send('\n');
    fdt_header *header = (fdt_header *)dtb_ptr;

    if (get_le2be_uint(&(header->magic)) != 0xd00dfeed)
    {
        uart_send_string("header magic != 0xd00dfeed\n");
        return -1;
    }
    uint32_t totalsize = get_le2be_uint(&(header->totalsize));
    uintptr_t struct_ptr = dtb_ptr + get_le2be_uint(&(header->off_dt_struct));
    uintptr_t strings_ptr = dtb_ptr + get_le2be_uint(&(header->off_dt_strings));
    parse_struct(cb, struct_ptr, strings_ptr, totalsize);

     return 0;
}
