CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
QEMU=qemu-system-arm
TRUNCATE=truncate
MKE2FS=/sbin/mke2fs
DEBUGFS=/sbin/debugfs

AS=$(CC)

CC+= -Wall -c -mcpu=arm926ej-s -marm -Werror
LDKERNEL=$(LD) -Map kernelmap.txt -T linkerscript.txt
LDPROGRAM=$(LD) -T linkerscript2.txt
AS+= -c -x assembler-with-cpp -mcpu=arm926ej-s
QEMUARGS=-machine integratorcp -kernel kernel.bin -serial stdio -sd sdcard.img
DISPLAY?=:0
export DISPLAY
SDL_STDIO_REDIRECT=no
export SDL_STDIO_REDIRECT

all:
	$(AS) kernelasm.s
	$(AS) interruptasm.s
	$(CC) kernelmain.c
	$(CC) console.c
	$(CC) testsuite.c
	$(CC) kprintf.c
	$(CC) util.c
	$(CC) disk.c
	$(CC) file.c
	$(CC) interrupt.c
	$(CC) timer.c
	$(LDKERNEL) -o kernel.tmp kernelasm.o interruptasm.o kernelmain.o console.o testsuite.o kprintf.o util.o disk.o file.o interrupt.o timer.o
	$(OBJCOPY) -Obinary kernel.tmp kernel.bin
	$(TRUNCATE) -s 400000000 sdcard.img
	$(MKE2FS) -b 4096 -F -I 128 -q -t ext2 -r 0 -L moocow -g 32768 sdcard.img
	$(AS) crtasm.s
	$(CC) crtc.c
	$(CC) print.c
	$(CC) usr_blink.c
	$(CC) tickingclock.c
	$(LDPROGRAM) -o print.tmp print.o
	$(LDPROGRAM) -o usr_blink.tmp usr_blink.o
	$(LDPROGRAM) -o tickingclock.tmp tickingclock.o
	$(OBJCOPY) -Obinary print.tmp print.bin 
	$(OBJCOPY) -Obinary usr_blink.tmp usr_blink.bin
	$(OBJCOPY) -Obinary tickingclock.tmp tickingclock.bin
	$(DEBUGFS) -w -f fscmd.txt sdcard.img
	"$(QEMU)" $(QEMUARGS) kernel.bin
	
	
clean:
	-/bin/rm *.o *.exe *.bin *.img *.tmp kernelmap.txt