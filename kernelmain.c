#include "console.h"
#include "disk.h"
#include "kprintf.h"
#include "util.h"

extern unsigned sbss, ebss;

void kernel_main()
{
	kmemset((void *)(&sbss), 0, &ebss - &sbss);
	
	disk_init();
	console_init();

	kprintf("Volume Label: %s\n", sblock.volname);
	kprintf("Free Blocks: %d\n", sblock.free_block_count);
	kprintf("Blocks per Group: %d\n", sblock.blocks_per_group);
	kprintf("Num Block Groups: %d\n", num_block_groups);

	int i;
	for (i = 0; i < num_block_groups; ++i) {
		kprintf("Block %d has %d free blocks\n", i, blockGroupDescriptors[i].free_blocks);
	}

	kprintf("Division Test: %d / %d = %d\n", 10, 3, kdivide(10, 3));
	kprintf("Division Test: %d / %d = %d\n", 10, 2, kdivide(10, 2));

	list_root();

	while(1) { }
}
