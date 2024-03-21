CFLAGS = -Wall -Wextra -O2 -ffreestanding -nostdinc -nostdlib -nostartfiles
CFLAGS += -IBoot -IUART/header -IUtils/header -IBootloader/header
CFLAGS += -I/usr/lib/gcc-cross/aarch64-linux-gnu/11/include -I/usr/lib/gcc-cross/aarch64-linux-gnu/11/include-fixed

K8=kernel8
LINKER=Boot/linker.ld
SRC_S=$(wildcard Boot/*.s UART/*.s Shell/*.s Mailbox/*.s Reboot/*.s Utils/*.s Bootloader/*.s)
SRC_C=$(wildcard UART/*.c Shell/*.c Mailbox/*.c Reboot/*.c Utils/*.c Bootloader/*.c)

OBJS=$(SRC_C:.c=.o) $(SRC_S:.s=.o) main.o

all: $(K8).img

$(K8).img: $(OBJS)
	aarch64-linux-gnu-ld -T $(LINKER) -o $(K8).elf $^
	aarch64-linux-gnu-objcopy -O binary $(K8).elf $@

%.o: %.s
	aarch64-linux-gnu-gcc $(CFLAGS) -c $< -o $@

%.o: %.c
	aarch64-linux-gnu-gcc $(CFLAGS) -c $< -o $@

sd: $(K8).img
	cp $(K8).img /media/ben/4DFF-0A36
	sync

qemu: 
	qemu-system-aarch64 -M raspi3b -serial null -serial stdio -serial pty -display none  -kernel kernel8.img

dump:
	@aarch64-linux-gnu-gcc $(CFLAGS) -c $(SRC_S) $(SRC_C)
	@aarch64-linux-gnu-ld -T $(LINKER) -o $(K8).elf $(OBJS)
	@aarch64-linux-gnu-objdump -h $(K8).elf

rpi3:
	screen /dev/ttyUSB0 115200
    
clean:
	rm -f $(wildcard */*.o) $(wildcard *.o) $(K8).elf $(K8).img