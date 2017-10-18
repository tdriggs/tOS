#include "console.h"
#include "disk.h"
#include "kprintf.h"
#include "util.h"

void sweet();

extern unsigned sbss, ebss;

void kernel_main()
{
	kmemset((void *)(&sbss), 0, &ebss - &sbss);
	
	disk_init();
	console_init();

	sweet();

	while(1) { }
}
