#include "file.h"

int file_open(const char * fname, int flags) {
	int file_index = 0;
	while (file_table[file_index].in_use == 0) {
		++file_index;

		if (file_index >= MAX_FILES) {
			return EMFILE;
		}
	}

	int inode_number = get_file_inode(2, fname);
	if (inode_number == 0) {
		return ENOENT;
	}

	disk_read_inode(inode_number, file_table[file_index].inode);

	kmemcpy and compare length

	file_table[file_index].in_use = 1;
	file_table[file_index].flags = flags;

	return file_index;
}

int file_close(int fd) {
	if (fd >= MAX_FILES) {
		return EINVAL;
	}

	file_table[fd].in_use = 0;
	return SUCCESS;
}

unsigned get_file_inode(unsigned dir_inode, const char* filename) {

}
