#pragma once

struct File {
	int in_use;
	struct Inode inode;
	int flags; // 0r, 1w, 2rt
};

#define MAX_FILES 16
struct File file_table[MAX_FILES]
