#include "file.h"

int file_open(const char * fname, int flags) {
	int file_index = 0;
	while (file_table[file_index].in_use == 1) {
		++file_index;

		if (file_index >= MAX_FILES) {
			return EMFILE;
		}
	}

	int inode_number = get_file_inode(2, fname);
	if (inode_number == 0) {
		return ENOENT;
	}

	disk_read_inode(inode_number, &file_table[file_index].inode);

	file_table[file_index].in_use = 1;
	file_table[file_index].flags = flags;

	return file_index;
}

int file_close(int fd) {
	if (fd >= MAX_FILES || fd < 0) {
		return EINVAL;
	}

	if (file_table[fd].in_use == 0) {
		return ENOFILE;
	}

	file_table[fd].in_use = 0;
	return SUCCESS;
}

unsigned get_file_inode(unsigned dir_inode, const char * filename) {
	struct Inode inode;
	disk_read_inode(dir_inode, &inode);
	
	static char buffer[4096];
	disk_read_block(inode.direct[0], buffer);
	struct DirEntry * dirEntry = (struct DirEntry *)buffer;

	while (dirEntry->rec_len) {
		disk_read_inode(dirEntry->inode, &inode);
		int mode = inode.mode >> 12;

		if (mode == 8) {
			int equal = 1;
			for (int i = 0; i < dirEntry->name_len; ++i) {
				if (dirEntry->name[i] != filename[i]) {
					equal = 0;
					break;
				}
			}

			if (equal == 1) {
				return dirEntry->inode;
			}
		}
		
		dirEntry = (struct DirEntry *)(((char *)dirEntry) + dirEntry->rec_len);	
	}

	return 0;
}
