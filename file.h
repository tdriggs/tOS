#pragma once

#include "errno.h"
#include "disk.h"

struct File {
	int in_use;
	struct Inode inode;
	int flags;
};

#define MAX_FILES 16
struct File file_table[MAX_FILES];

int file_open(const char * fname, int flags);
int file_close(int fd);
unsigned get_file_inode(unsigned dir_inode, const char * filename);
