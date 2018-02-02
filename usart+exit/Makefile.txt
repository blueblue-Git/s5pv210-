CC		= arm-linux-gcc
LD 		= arm-linux-ld
OBJCOPY = arm-linux-objcopy
OBJDUMP = arm-linux-objdump
AR 		= arm-linux-arm-linux-ar


INCDIR := $(shell pwd)

CPPFLAGS := -nostdlib -nostdinc -I$(INCDIR)/include 
CFLAGS   := -Wall -O2 -fno_builtin

export CC LD OBJCOPY OBJDUMP AR CPPFLAGS CFLAGS

objs := start.o  clock.o  uart.o main.o led.o int.o exit.o
objs += lib/libc.a

test.bin: $(objs)
	$(LD) -Tlink.lds -o test.elf $^
	$(OBJCOPY) -O binary test.elf test.bin	
	$(OBJDUMP) -D test.elf > test_elf.dis
	gcc mkv210_image.c -o mkx210
	./mkx210 test.bin   test_sd.bin
	
lib/libc.a
	cd lib;make ;cd ..
%.o : %.S
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c
%.o : %.c 
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $< -c
	
clean：
	rm *.o  *.elf *.bin *.dis mkx210 -f
	cd lib;make clean; cd ..