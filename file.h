#pragma once

#include "errno.h"
#include "disk.h"
#include "util.h"

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

struct File {
	int offset;
	int in_use;
	struct Inode inode;
	int flags;
};

#define MAX_FILES 16
struct File file_table[MAX_FILES];

int file_open(const char * fname, int flags);
int file_close(int fd);
unsigned get_file_inode(unsigned dir_inode, const char * filename);
int file_read(int fd, void * buffemakr, int count);
int file_write(int fd, const void * buffer, int count);
int file_seek(int fd, int offset, int whence);
int file_exec(const char * fname);