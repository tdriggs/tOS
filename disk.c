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
	static char blockbuffer[4096];

	disk_read_block(0, blockbuffer);
	kmemcpy(&sblock, blockbuffer + 1024, sizeof(sblock));

	num_block_groups = kdivide(sblock.block_count, sblock.blocks_per_group);

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
	disk_read_block_partial(block, datablock, 0, 4096);
}

void disk_read_block_partial(unsigned block, const void * p, unsigned start, unsigned count) {
	static char buffer[4096];
	int i;
	for (i = 0; i < 8; ++i) {
		disk_read_sector((block * 8) + i, buffer + (512 * i));
	}
	kmemcpy((void *)p, buffer + start, count);
}

void disk_read_inode(unsigned inode_number, struct Inode * inode) {
	--inode_number;
	int inode_group_number = kdivide(inode_number, sblock.inodes_per_group);
	int inode_block_number = sblock.blocks_per_group * inode_group_number + 4;
	int byte_offset = kmodulo(inode_number, sblock.inodes_per_group) * sizeof(inode);
	int block_offset = kdivide(byte_offset, 4096);
	int inode_offset = inode_number % 32;

	disk_read_block_partial(inode_block_number + block_offset, inode, inode_offset * sizeof(struct Inode), sizeof(struct Inode));
}

void list_directory(int inode_number, int depth) {
	struct Inode inode;
	disk_read_inode(inode_number, &inode);
	
	static char buffer[4096];
	disk_read_block(inode.direct[0], buffer);
	struct DirEntry * dirEntry = (struct DirEntry *)buffer;

	while (dirEntry->rec_len) {
		disk_read_inode(dirEntry->inode, &inode);
		int mode = inode.mode >> 12;

		kprintf("Mode: %d", mode);

		if (mode == 4) {
			if (dirEntry->name_len == 2 && dirEntry->name[0] == '.' && dirEntry->name[1] == '.') {
				// kprintf("DOTDOT");
			}
			else if (dirEntry->name[0] == '.' && dirEntry->name_len == 1) {
				// kprintf("DOT");
			} else {
				// list_directory(dirEntry->inode, depth + 1);
				// kprintf("RECURSE");
			}
		} 


		int i;
		for (i = 0; i < depth; ++i) {
			kprintf("  ");
		}

		kprintf("-%.*s\n", dirEntry->name_len, dirEntry->name);
		dirEntry = (struct DirEntry *)(((char *)dirEntry) + dirEntry->rec_len);	
	}
}

void list_root() {
	kprintf("-/\n");
	list_directory(2, 1);
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
