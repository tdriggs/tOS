#include "console.h"
#include "disk.h"
#include "interrupt.h"
#include "kprintf.h"
#include "file.h"
#include "timer.h"
#include "util.h"

void sweet();

extern unsigned sbss, ebss;

void kernel_main()
{
	kmemset((void *)(&sbss), 0, &ebss - &sbss);
	
	disk_init();
	console_init();
	interrupt_init();
	timer_init();

	//sweet(1, framebuffer_start);
	file_exec("tickingclock.bin");

	halt();
}
