CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
QEMU=qemu-system-arm
TRUNCATE=truncate
MKE2FS=/sbin/mke2fs
DEBUGFS=/sbin/debugfs

AS=$(CC)

CC+= -Wall -c -mcpu=arm926ej-s -marm -Werror
LD+=-Map kernelmap.txt -T linkerscript.txt
AS+= -c -x assembler-with-cpp -mcpu=arm926ej-s
QEMUARGS=-machine integratorcp -kernel kernel.bin -serial stdio -sd sdcard.img
DISPLAY?=:0
export DISPLAY
SDL_STDIO_REDIRECT=no
export SDL_STDIO_REDIRECT

all:
	$(AS) kernelasm.s
	$(CC) kernelmain.c
	$(CC) console.c
	$(CC) testsuite.c
	$(CC) kprintf.c
	$(CC) util.c
	$(CC) disk.c
	$(LD) -o kernel.tmp kernelasm.o kernelmain.o console.o testsuite.o kprintf.o util.o disk.o
	$(OBJCOPY) -Obinary kernel.tmp kernel.bin
	$(TRUNCATE) -s 400000000 sdcard.img
	$(MKE2FS) -b 4096 -F -I 128 -q -t ext2 -r 0 -L moocow -g 32768 sdcard.img
	$(DEBUGFS) -w -f fscmd.txt sdcard.img
	"$(QEMU)" $(QEMUARGS) kernel.bin
	
	
clean:
	-/bin/rm *.o *.exe *.bin *.img *.tmp kernelmap.txt