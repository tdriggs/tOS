#include "console.h"
#include "disk.h"
#include "kprintf.h"

void sweet();

void kernel_main()
{
	disk_init();
	console_init();

	// sweet();

	kprintf("Volume Label: %s\n", sblock.volname);
	kprintf("Free Blocks: %d\n", sblock.free_block_count);
	kprintf("Blocks per Group: %d\n", sblock.blocks_per_group);
	kprintf("Num Block Groups: %d\n", num_block_groups);

	int i;
	for (i = 0; i < num_block_groups; ++i) {
		kprintf("Block %d has %d free blocks\n", i, blockGroupDescriptors[i].free_blocks);
	}
	
	while(1) { }
}
