#include "disk.h"

int is_busy() {
	return *STATUS & (1 << 24);
}

void disk_init() {
	*POWER = 3;
	*CLOCK = 8;
	*CMD = (1 << 10);
	*CMD = 8 | (1 << 10);

	do {
	    *CMD = 55 | (1 << 10) | (1 << 6);
	    *ARG = 0xffffffff;
	    *CMD = 41 | (1 << 10) | (1 << 6);
	} while(is_busy());

	*CMD = 2 | (1 << 10) | (1 << 6) | (1 << 7);
	*CMD = 3 | (1 << 10) | (1 << 6);
	unsigned relative_address = *RESPONSE;
	*ARG = relative_address;
	*CMD = 7 | (1 << 10) | (1 << 6);

	load_filesystem();
}

void load_filesystem() {
	disk_read_block(0, blockbuffer);
	kmemcpy(&sblock, blockbuffer + 1024, sizeof(sblock));

	num_block_groups = 0;
	int num_blocks = sblock.block_count;
	while (num_blocks > 0) {
		num_blocks -= sblock.blocks_per_group;
		++num_block_groups;
	}

	disk_read_block(1, blockbuffer);
	kmemcpy(blockGroupDescriptors, blockbuffer, 
		sizeof(struct BlockGroupDescriptor) * num_block_groups);
}

void disk_read_sector(unsigned sector, const void * datablock) {
	*DATA_LENGTH = 512;
	*DATA_TIMER = 100;
	*DATA_CONTROL = 1 | (1 << 1) | (9 << 4);
	*ARG = 512 * sector;     
	*CLEAR = 0x3ff;
	*CMD = 17 | (1<<10) | (1<<6);

	unsigned * p = (unsigned *)datablock;
	int k;
	for(k = 0; k < 128; ++k){
	    while(*STATUS & (1 << 19));

	    *CLEAR = 0x3ff;
	    unsigned v = *DATA_FIFO;
	    *p++ = v;
	}
}

void disk_read_block(unsigned block, const void * datablock) {
	int i;
	for (i = 0; i < 8; ++i) {
		disk_read_sector((block * 8) + i, datablock + (512 * i));
	}
}

void disk_write_sector(unsigned sector, const void * datablock) {
	*DATA_LENGTH = 512; 
	*DATA_TIMER = 100;
	*DATA_CONTROL = 1 | (9 << 4);
	*ARG = 512 * sector;
	*CLEAR = 0x3ff;
	*CMD = 24 | (1 << 10) | (1 << 6); 

	unsigned * p = (unsigned *)datablock;
	int k;
	for(k = 0; k < 128; ++k) {
	    while((*STATUS & (1 << 20)));

	    *CLEAR = 0x3ff;
	    *DATA_FIFO = *p;
	    p++;
	}
}
