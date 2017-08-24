
CC=arm-none-eabi-gcc
LD=arm-none-eabi-ld
OBJCOPY=arm-none-eabi-objcopy
QEMU=qemu-system-arm


# CC=/usr/arm-linux-gnu-gcc
# LD=arm-linux-gnu-ld
# OBJCOPY=arm-linux-gnu-objcopy


AS=$(CC)

CC+= -Wall -c -mcpu=arm926ej-s -marm -Werror
LD+=-Map kernelmap.txt -T linkerscript.txt
AS+= -c -x assembler-with-cpp -mcpu=arm926ej-s
QEMUARGS=-machine integratorcp -kernel kernel.bin -serial stdio
DISPLAY?=:0
export DISPLAY
SDL_STDIO_REDIRECT=no
export SDL_STDIO_REDIRECT

all: 
	$(AS) kernelasm.s
	$(LD) -o kernel.tmp kernelasm.o
	$(OBJCOPY) -Obinary kernel.tmp kernel.bin
	"$(QEMU)" $(QEMUARGS) kernel.bin
	
	
clean:
	-/bin/rm *.o *.exe *.bin *.img *.tmp