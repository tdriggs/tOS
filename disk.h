#pragma once

#include "util.h"
#include "kprintf.h"

#define MMCP_START ((volatile unsigned*) 0x1c000000)
#define POWER ( MMCP_START)
#define CLOCK (MMCP_START+1)
#define ARG (MMCP_START+2)
#define CMD (MMCP_START+3)
#define RESPONSE_CMD (MMCP_START+4)
#define RESPONSE (MMCP_START+5)
#define DATA_TIMER (MMCP_START+9)
#define DATA_LENGTH (MMCP_START+10)
#define DATA_CONTROL (MMCP_START+11)
#define DATA_COUNTER (MMCP_START+12)
#define STATUS (MMCP_START+13)
#define CLEAR (MMCP_START+14)
#define INTERRUPT0_MASK (MMCP_START+15)
#define INTERRUPT1_MASK (MMCP_START+16)
#define SELECT (MMCP_START+17)
#define FIFO_COUNT (MMCP_START+18)
#define DATA_FIFO (MMCP_START+32)

#pragma pack(push,1)
struct Superblock {
    unsigned inode_count;
    unsigned block_count;
    unsigned r_block_count;
    unsigned free_block_count;
    unsigned free_inode_count;
    unsigned first_data_block;
    unsigned logical_block_size;
    unsigned logical_fragment_size;
    unsigned blocks_per_group;
    unsigned fragments_per_group;
    unsigned inodes_per_group;
    unsigned mounttime; 
    unsigned writetime;
    unsigned short mountcount;
    unsigned short maxmountcount;
    unsigned short magic;
    unsigned short state;
    unsigned short errors;
    unsigned short minorrev;
    unsigned lastcheck;
    unsigned checktime;
    unsigned creator;
    unsigned revision;
    unsigned short resuid;
    unsigned short resgid;
    unsigned first_inode;
    unsigned short inode_size;
    unsigned short block_group_number;
    unsigned feature_compat;
    unsigned feature_incompat;
    unsigned feature_ro_compat;
    unsigned char uuid[16];
    char volname[16];
    char lastmount[64];
    char reserved[824];
};
#pragma pack(pop)

#pragma pack(push,1)
struct BlockGroupDescriptor {
    unsigned block_bitmap;
    unsigned inode_bitmap;
    unsigned inode_table;
    unsigned short free_blocks;
    unsigned short free_inodes;
    unsigned short used_dirs;
    unsigned short pad;
    char reserved[12];
};
#pragma pack(pop)

#pragma pack(push,1)
struct Inode {
    unsigned short mode;
    unsigned short uid;
    unsigned size;
    unsigned atime;
    unsigned ctime;
    unsigned mtime;
    unsigned dtime;
    unsigned short gid;
    unsigned short links;
    unsigned blocks;
    unsigned flags;
    unsigned osd1;
    unsigned direct[12];
    unsigned indirect;
    unsigned doubleindirect;
    unsigned tripleindirect;
    unsigned generation;
    unsigned fileacl;
    unsigned diracl;
    unsigned osd2;
    char reserved[12];
}; 
#pragma pack(pop)

#pragma pack(push,1)
struct DirEntry {
    unsigned inode;
    unsigned short rec_len;
    unsigned short name_len;
    char name[1];
};
#pragma pack(pop)

struct Superblock sblock;
int num_block_groups;
struct BlockGroupDescriptor * blockGroupDescriptors;
struct DirEntry * rootDirEntries;

int is_busy();

void disk_init();

void load_filesystem();

void disk_read_sector(unsigned sector, const void * datablock);

void disk_read_block(unsigned block, const void * datablock);

void disk_read_inode(unsigned num, struct Inode * inode);

void disk_read_block_partial(unsigned block, const void * p, unsigned start, unsigned count);

void list_directory(int inode_number, int depth);

void list_root();

void disk_write_sector(unsigned sector, const void * datablock);
