#include "File.h"

//int file_open(const char * fname, int flags) {
	// Find entry in table where in_use == 0 (for loop) else return negatve error code
	// when found 
	// int inode_number = get_file_inode(root, param) returns inode # or 0
	// if inode_number = 0 return error code
	// disk read inode(inode_number, file_table[i].ino)
	// kmemcpy and compare length
	// set in_use to 1
	// set flags to flags
	// return error code
//}

// int file_close(int fd) {
// 	// set in_use to 0 return error code
// }