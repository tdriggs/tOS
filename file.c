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
	file_table[file_index].offset = 0;

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

int file_read(int fd, void * buffer, int count) {
	if (fd >= MAX_FILES || fd < 0) {
		return EINVAL;
	}

	struct File * fp = &file_table[fd];

	if (fp->in_use == 0) {
		return ENOFILE;
	}

	if (count <= 0 || fp->offset >= fp->inode.size) {
		return 0;
	}

	static char tmpbuffer[4096];
	static unsigned int U[1024];

	int bi = kdivide(fp->offset, 4096);
	if (bi < 12) {
		disk_read_block(fp->inode.direct[bi], tmpbuffer);
	} else {
		bi -= 12;
		if (bi < 1024) {
			disk_read_block(fp->inode.indirect, (void*)U);
			disk_read_block(U[bi], tmpbuffer);
		}
		else {
			bi -= 1024;
			if (bi < 1048576) {
				disk_read_block(fp->inode.doubleindirect, (void*)U);
				disk_read_block(U[bi >> 10], (void*)U);
				disk_read_block(U[kmodulo(bi, 1024)], tmpbuffer);
			}
		}
	}

	int bo = kmodulo(fp->offset, 4096);

	int final_count = count;
	int buffer_size = 4096 - bo;
	int remaining_file_size = fp->inode.size - fp->offset;

	if (buffer_size < final_count) {
		final_count = buffer_size;
	}
	if (remaining_file_size < final_count) {
		final_count = remaining_file_size;
	}

	kmemcpy(buffer, tmpbuffer + bo, final_count);
	fp->offset += final_count;

	return final_count;
}

int file_write(int fd, const void * buffer, int count) {
	return ENOSYS;
}

int file_seek(int fd, int offset, int whence) {
	if (fd >= MAX_FILES || fd < 0) {
		return EINVAL;
	}

	struct File * fp = &file_table[fd];

	if (fp->in_use == 0) {
		return ENOFILE;
	}

	if (whence == SEEK_SET) {
		if (offset < 0) {
			return EINVAL;
		}

		fp->offset = offset;
		return SUCCESS;
	} else if (whence == SEEK_CUR) {
		if (fp->offset + offset < 0) {
			return EINVAL;
		}

		fp->offset += offset;
		return SUCCESS;
	} else if (whence == SEEK_END) {
		if (offset + (int)fp->inode.size < 0) {
			return EINVAL;
		}

		fp->offset = fp->inode.size + offset;
		return SUCCESS;
	} else {
		return EINVAL;
	}

	return SUCCESS;
}

int file_exec(const char * fname) {
	int fd = file_open(fname, 0);
	if (fd < 0) {
		return EINVAL;
	}

	int i = 0;
	int result = file_read(fd, (void*)0x400000, 100);
	i += result;
	while (result != 0) {
		result = file_read(fd, (void*)0x400000 + i, 100);
		i += result;
	}

	file_close(fd);

	asm volatile(
		"mrs r0,cpsr\n"
		"and r0,#0xffffffe0\n"
		"orr r0,#0x10\n"
		"msr cpsr,r0\n"
		"mov sp,#0x800000\n"
		"mov pc,#0x400000\n"
	);

	return SUCCESS;
}
