"An Ext2 file systems starts with a superblock located at byte offset 1024 from the start of the volume."
"This is block 1 for a 1KiB block formatted volume or within block 0 for larger block sizes. Note that the"
"size of the superblock is constant regardless of the block size."
"On the next block(s) following the superblock, is the Block Group Descriptor Table; which ."

** provides an overview of how the volume is split into block groups and where 
** to find the inode bitmap, the block bitmap, and the inode table for each block group

** [REVISION 0] of Ext2, each block group consists of a copy superblock, a copy of the block group
** descriptor table, a block bitmap, an inode bitmap, an inode table, and data blocks.

** [REVISION 1] and the sparse superblock feature in Ext2, only specific block groups
** contain copies of the superblock and block group descriptor table. All block groups still contain the
** block bitmap, inode bitmap, inode table, and data bloc

** The block bitmap and inode bitmap are limited to 1 block each per block group, so the total blocks per
** block group is therefore limited. (More information in the Block Size Impact table).

BLK_SIZE

EXT2: 
    
    [0    -> 512 ] boot record (if present) 
    [512  -> 1024] additional boot record data (if present)

    [ BLK_SIZE -> BLK_SIZE ]        superblock 
    [ BLK_SIZE -> BLK_SIZE ]        Block Group Descriptor Table
    [ BLK_SIZE -> BLK_SIZE ]        block bitmap
    [ BLK_SIZE -> BLK_SIZE ]        inode bitmap
    [ BLK_SIZE -> BLK_SIZE * ? ]    inode table
    [ BLK_SIZE -> BLK_SIZE * ? ]    data blocks


////////////////////////////////////////////////////////////////////////
Floppy Disk Layout, 1KiB blocks

Block Offset        | Length            | Description
--------------------|-------------------|-------------------------------
byte 0              |512 bytes          |boot record (if present)
byte 512            |512 bytes          |additional boot record data (if present)

*************** block group 0, blocks 1 to 1439 ************************
byte 1024           |1024 bytes         |superblock
block 2             |1 block            |block group descriptor table
block 3             |1 block            |block bitmap
block 4             |1 block            |inode bitmap
block 5             |23 blocks          |inode table
block 28            |1412 blocks        |data blocks

////////////////////////////////////////////////////////////////////////
20MB ext2 file system, using 1KiB blocks

Block Offset        | Length            | Description
--------------------|-------------------|-------------------------------
byte 0              |512 bytes          |boot record (if present)
byte 512            |512 bytes          |additional boot record data (if present)

*************** block group [0], blocks [1 to 8192] ********************
block 1             |1 block            |superblock
block 2             |1 block            |block group descriptor table
block 3             |1 block            |block bitmap
block 4             |1 block            |inode bitmap
block 5             |214 blocks         |inode table
block 219           |7974 blocks        |data blocks

*************** block group [1], blocks [8193 to 16384] ****************
block 8193          |1 block            |superblock backup       [COPY]
block 8194          |1 block            |block group descriptor table backup
block 8195          |1 block            |block bitmap
block 8196          |1 block            |inode bitmap
block 8197          |214 blocks         |inode table
block 8408          |7974 blocks        |data blocks

*************** block group [2], blocks [16385 to 24576] ***************
block 16385         |1 block            |block bitmap            [COPY]
block 16386         |1 block            |inode bitmap
block 16387         |214 blocks         |inode table
block 16601         |3879 blocks        |data blocks

////////////////////////////////////////////////////////////////////////

Notice how block 0 is not part of the block group 0 in 1KiB block size file systems. The reason for this is
block group 0 always starts with the block containing the [superblock]. Hence, on 1KiB block systems,
block group 0 starts at block 1, but on larger block sizes it starts on block 0. For more information, see
the [s_first_data_block] [superblock] entry.


-------------------------------------
s_inodes_count: | 32-bit | total number of inodes, used + free, in the file system. 

(s_inodes_per_group * number of block groups) 
|| 
( sum of the inodes defined in each block group )

-------------------------------------
s_free_inodes_count: | 32-bit

(sum of all free inodes of all the block groups)

-------------------------------------
s_first_data_block: | 32-bit | (id of the block containing the superblock structure).

always [0] for block size larger than 1KB, 
always [1] for block size of 1KB

-------------------------------------
s_log_block_size:

block_size = 1024 << ext2_super.s_log_block_size;

-------------------------------------
s_log_frag_size:

if: ext2_super.s_log_frag_size > 0
    frag_size = 1024 << ext2_super.s_log_frag_size
else: 
    frag_size = 1024 >> -ext2_super.s_log_frag_size;

-------------------------------------
s_frags_per_group: | 32-bit | total number of fragments per group. 

also used to determine the size of the [block bitmap] of each [block group]

-------------------------------------
s_inodes_per_group: | 32-bit 

also used to determine the size of the [inode bitmap] of each [block group]. 
Note that you cannot have more than (block size in bytes * 8) inodes per group as the [inode bitmap] 
must fit within a single block. 

This value must be a perfect multiple of the number of inodes that can fit in a block 

((1024 << s_log_block_size) / s_inode_size )

-------------------------------------
s_magic: | EXT2_SUPER_MAGIC | 0xEF53

-------------------------------------
s_state:

EXT2_VALID_FS | [1] | Unmounted cleanly
EXT2_ERROR_FS | [2] | Errors detected

-------------------------------------
s_errors: | 16-bit | what the file system driver should do when an error is detected. 

EXT2_ERRORS_CONTINUE | [1] | continue as if nothing happened
EXT2_ERRORS_RO       | [2] | remount read-only
EXT2_ERRORS_PANIC    | [3] | cause a kernel panic

-------------------------------------
s_first_ino: | 32-bit | index to the first inode useable for standard files. 

[revision 0]: the first non-reserved inode is fixed to 11 (EXT2_GOOD_OLD_FIRST_INO). 
[revision 1]: and later this value may be set to any value.

-------------------------------------
s_inode_size: | 16-bit | size of the inode structure. 

[revision 0]: this value is always 128 (EXT2_GOOD_OLD_INODE_SIZE). 
[revision 1]: and later, this value must be a perfect power of 2
and must be smaller or equal to the block size (1 << s_log_block_size )

-------------------------------------
s_feature_compat: | 32-bit | bitmask of compatible features. 

The file system implementation is free to support them or not without risk of damaging the meta-data

EXT2_FEATURE_COMPAT_DIR_PREALLOC    | 0x0001    | Block pre-allocation for new directories
EXT2_FEATURE_COMPAT_IMAGIC_INODES   | 0x0002    | *****
EXT3_FEATURE_COMPAT_HAS_JOURNAL     | 0x0004    | An Ext3 journal exists
EXT2_FEATURE_COMPAT_EXT_ATTR        | 0x0008    | Extended inode attributes are present
EXT2_FEATURE_COMPAT_RESIZE_INO      | 0x0010    | Non-standard inode size used
EXT2_FEATURE_COMPAT_DIR_INDEX       | 0x0020    | Directory indexing (HTree)

-------------------------------------
s_uuid: | 128-bit | value used as the volume id. 
This should, as much as possible, be unique for each file system formatted.

-------------------------------------
s_volume_name: | 16-bytes | volume name, mostly unusued. 
A valid volume name would consist of only ISO-Latin-1 characters and be 0 terminated.

-------------------------------------
s_algo_bitmap: | 32-bit | value used by compression algorithms to determine the compression method(s) used.

Constant Name       | Bit Number    | Description

EXT2_LZV1_ALG       |0              | bin val 0x00000001
EXT2_LZRW3A_ALG     |1              | bin val 0x00000002
EXT2_GZIP_ALG       |2              | bin val 0x00000004
EXT2_BZIP2_ALG      |3              | bin val 0x00000008
EXT2_LZO_ALG        |4              | bin val 0x00000010

-------------------------------------
s_prealloc_blocks: | 8-bit | 

value representing the number of blocks the implementation 
should attempt to pre-allocate whencreating a new regular file.

-------------------------------------
s_prealloc_dir_blocks: | 8-bit | 

value representing the number of blocks the implementation should 
attempt to pre-allocate when creating a new directory.
