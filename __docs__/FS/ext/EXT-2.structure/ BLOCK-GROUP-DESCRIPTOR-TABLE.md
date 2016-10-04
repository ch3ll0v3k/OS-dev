// The Second Extended File System.pdf
// 3.2. Block Group Descriptor Table

The block group descriptor table is an array of block group descriptor, used to define parameters of all
the block groups. It provides the location of the inode bitmap and inode table, block bitmap, number of
free blocks and inodes, and some other useful information.

The block group descriptor table starts on the first block following the [SUPERBLOCK]. This would be the
third block on a 1KiB block file system, or the second block for 2KiB and larger block file systems.
Shadow copies of the block group descriptor table are also stored with every copy of the [SUPERBLOCK].

Depending on how many block groups are defined, this table can require multiple blocks of storage.
Always refer to the [SUPERBLOCK] in case of doubt


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

// [BLOCK GROUP DESCRIPTOR STRUCTURE]

Offset (bytes)  | Size (bytes)  | Description
----------------|---------------|----------------------------
0               | 4             | bg_block_bitmap
4               | 4             | bg_inode_bitmap
8               | 4             | bg_inode_table
12              | 2             | bg_free_blocks_count
14              | 2             | bg_free_inodes_count
16              | 2             | bg_used_dirs_count
18              | 2             | bg_pad
20              | 12            | bg_reserved
----------------|---------------|----------------------------


For each [BLOCK GROUP] in the file system, such a [GROUP_DESC] is created. 
Each represent a single [BLOCK GROUP] within the file system and the information within any one 
of them is pertinent only to the group it is describing. 
Every [BLOCK GROUP DESCRIPTOR TABLE] contains all the information about all the block groups.

NOTE: All indicated “block id” are absolute.

////////////////////////////////////////////////////////////////////////
bg_block_bitmap:        | 32-bit    | 

block id of the first block of the [BLOCK BITMAP] for the group represented.
Actual [BLOCK BITMAP] is located within its own allocated blocks starting at the block ID specified by this value

bg_inode_bitmap:        | 32-bit    | block id of the first block of the [INODE BITMAP] for the group represented.
bg_inode_table:         | 32-bit    | block id of the first block of the [INODE TABLE] for the group represented.
bg_free_blocks_count:   | 16-bit    | total number of free [BLOCKS] for the represented group.
bg_free_inodes_count:   | 16-bit    | total number of free [INODES] for the represented group.
bg_used_dirs_count:     | 16-bit    | number of inodes allocated to directories for the represented group.
bg_pad:                 | 16-bit    | used for padding the structure on a 32-bit boundary.
bg_reserved:            | 12-bytes  | reserved space for future revisions.

-------------------------------------
3.3. Block Bitmap:

On small file systems, the “Block Bitmap” is normally located at the first block, or second block if a
[SUPERBLOCK] backup is present, of each block group. Its official location can be determined by reading the
“bg_block_bitmap” in its associated group descriptor.


-------------------------------------
3.4. Inode Bitmap

The “Inode Bitmap” works in a similar way as the “Block Bitmap”, difference being in each bit
representing an inode in the “Inode Table” rather than a block.
There is one inode bitmap per group and its location may be determined by reading the
“bg_inode_bitmap” in its associated group descriptor.
When the inode table is created, all the reserved inodes are marked as used. In revision 0 this is the first
11 inodes.

-------------------------------------
3.5. Inode Table

The inode table is used to keep track of every directory, regular file, symbolic link, or special file; their
location, size, type and access rights are all stored in inodes. There is no filename stored in the inode
itself, names are contained in directory files only.

There is one inode table per block group and it can be located by reading the bg_inode_table in its
associated group descriptor. There are s_inodes_per_group inodes per table.

Each inode contain the information about a single physical file on the system. A file can be a directory, a
socket, a buffer, character or block device, symbolic link or a regular file. So an inode can be seen as a
block of information related to an entity, describing its location on disk, its size and its owner. An inode
looks like this:

typedef struct {
    uint16_t   i_mode;                      // [I_NODE] File mode */
    uint16_t   i_uid;                       // [I_NODE] Owner Uid */
    uint32_t   i_size;                      // [I_NODE] Size in bytes */
    uint32_t   i_atime;                     // [I_NODE] Access time */
    uint32_t   i_ctime;                     // [I_NODE] Creation time */
    uint32_t   i_mtime;                     // [I_NODE] Modification time */
    uint32_t   i_dtime;                     // [I_NODE] Deletion Time */
    uint16_t   i_gid;                       // [I_NODE] Group Id */
    uint16_t   i_links_count;               // [I_NODE] Links count */
    uint32_t   i_blocks;                    // [I_NODE] Blocks count */
    uint32_t   i_flags;                     // [I_NODE] File flags */
    union {
        struct {
            uint32_t  l_i_reserved1;
        } linux1;
        struct {
            uint32_t  h_i_translator;
        } hurd1;
        struct {
            uint32_t  m_i_reserved1;
        } masix1;
    } osd1;                                 // [I_NODE] OS dependent 1 */
    uint32_t   i_block[ EXT2_DIRECT_BLOCKS ];      // [I_NODE] Pointers to blocks */
    uint32_t   i_version;                   // [I_NODE] File version (for NFS) */
    uint32_t   i_file_acl;                  // [I_NODE] File ACL */
    uint32_t   i_dir_acl;                   // [I_NODE] Directory ACL */
    uint32_t   i_faddr;                     // [I_NODE] Fragment address */
    union {
        struct {
            uint8_t    l_i_frag;            // [I_NODE] Fragment number */
            uint8_t    l_i_fsize;           // [I_NODE] Fragment size */
            uint16_t   i_pad1;
            uint32_t   l_i_reserved2[2];
        } linux2;
        struct {
            uint8_t    h_i_frag;            // [I_NODE] Fragment number */
            uint8_t    h_i_fsize;           // [I_NODE] Fragment size */
            uint16_t   h_i_mode_high;
            uint16_t   h_i_uid_high;
            uint16_t   h_i_gid_high;
            uint32_t   h_i_author;
        } hurd2;
        struct {
            uint8_t    m_i_frag;            // [I_NODE] Fragment number */
            uint8_t    m_i_fsize;           // [I_NODE] Fragment size */
            uint16_t   m_pad1;
            uint32_t   m_i_reserved2[2];
        } masix2;
    } osd2;                                 // [I_NODE] OS dependent 2 */

    //} __attribute__ ((packed)) ext2_inode_t;
} ext2_inode_t;

-------------------------------------
i_mode: | 32-bit |

Constant        | Value     | Description
----------------|-----------|-------------------
** file format 
----------------|-----------|-------------------
EXT2_S_IFSOCK   | 0xC000    | socket
EXT2_S_IFLNK    | 0xA000    | symbolic link
EXT2_S_IFREG    | 0x8000    | regular file
EXT2_S_IFBLK    | 0x6000    | block device
EXT2_S_IFDIR    | 0x4000    | directory
EXT2_S_IFCHR    | 0x2000    | character device
EXT2_S_IFIFO    | 0x1000    | fifo
----------------|-----------|-------------------
** process execution user/group override 
----------------|-----------|-------------------
EXT2_S_ISUID    | 0x0800    | Set process User ID
EXT2_S_ISGID    | 0x0400    | Set process Group ID
EXT2_S_ISVTX    | 0x0200    | sticky bit
----------------|-----------|-------------------
** access rights 
----------------|-----------|-------------------
EXT2_S_IRUSR    | 0x0100    | user read
EXT2_S_IWUSR    | 0x0080    | user write
EXT2_S_IXUSR    | 0x0040    | user execute
EXT2_S_IRGRP    | 0x0020    | group read
EXT2_S_IWGRP    | 0x0010    | group write
EXT2_S_IXGRP    | 0x0008    | group execute
EXT2_S_IROTH    | 0x0004    | others read
EXT2_S_IWOTH    | 0x0002    | others write
EXT2_S_IXOTH    | 0x0001    | others execute
----------------|-----------|-------------------


-------------------------------------
i_size:

[revision 0]: (signed) 32bit | value indicating the size of the file in bytes. 
[revision 1]: and later revisions, and only for regular files, this represents the 
lower 32-bit of the file size; the upper 32-bit is located in the [i_dir_acl].

-------------------------------------
i_atime: | 32-bit |
i_ctime: | 32-bit |
i_mtime: | 32-bit |
i_dtime: | 32-bit |

-------------------------------------
i_blocks: | 32-bit | total number of 512-bytes blocks reserved to contain the data of this inode,

regardless if these blocks are used or not. 
The block numbers of these reserved blocks are contained in the [i_block] array.

Since this value represents 512-byte blocks and not file system blocks, this value should not be directly
used as an index to the [i_block] array. 
Rather, the maximum index of the i_block array should be computed from: 

i_blocks / ((1024 << s_log_block_size)/512);

or once simplified: 
i_blocks / ( 2 << s_log_block_size );

-------------------------------------
!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
i_block: [15 x 32-bit] | block numbers pointing to the blocks containing the data for this inode. 

The first [12 blocks] are direct blocks. 

**********
The 13th entry in this array is the block number of the first [INDIRECT BLOCK]; 

which is a block containing an array of block ID containing the data. 
Therefore, the 13th block of the file will be the first block ID contained in the [INDIRECT BLOCK]. 
With a 1KiB block size, blocks 13 to 268 of the file data are contained in this [INDIRECT BLOCK].

**********
The 14th entry in this array is the block number of the first [DOUBLY-INDIRECT BLOCK]; 

which is a block containing an array of [INDIRECT BLOCK] IDs, with each of those [INDIRECT BLOCK]s 
containing an array of blocks containing the data. 
In a 1KiB block size, there would be 256 [INDIRECT BLOCK]s per [DOUBLY-INDIRECT BLOCK],

with 256 direct blocks per [INDIRECT BLOCK] for a total of 65536 blocks per [DOUBLY-INDIRECT BLOCK].

**********
The 15th entry in this array is the block number of the [TRIPLY-INDIRECT BLOCK]; 
which is a block containing an array of [DOUBLY-INDRECT BLOCK] IDs, with each of those [DOUBLY-INDRECT BLOCK] 
containing an array of indrect block, and each of those [INDIRECT BLOCK] containing an array of direct block. 

In a 1KiB file system, this would be a total of 16777216 blocks per [TRIPLY-INDIRECT BLOCK].
A value of 0 in this array effectively terminates it with no further block being defined. All the remaining
entries of the array should still be set to 0.

-------------------------------------
i_file_acl: | 32-bit | value indicating the block number containing the extended attributes. 
[REVISION 0]: this value is always 0

-------------------------------------
i_dir_acl: 
[revision 0]: this 32bit value is always 0. 
[revision 1]: for regular files this 32bit value contains the high 32 bits of the 64bit file size

********************************************************************************************
********************************************************************************************
********************************************************************************************
[ LOCATING AN INODE ]

Inodes are all numerically ordered. 
The [INODE NUMBER] is an index in the [INODE TABLE] to an inode structure. 

The size of the [INODE TABLE] is fixed at format time; it is built to hold a maximum number of entries. 

Due to the large amount of entries created, the table is quite big and thus, it is split equally
among all the [BLOCK GROUPS] (see Chapter 3 for more information)


The [s_inodes_per_group] field in the [SUPERBLOCK] structure tells us how many inodes are defined per group.

Knowing that inode 1 is the first inode defined in the [INODE TABLE], one can use the following formulaes:
-----------------------------------------------------
block group = (inode - 1) / s_inodes_per_group
-----------------------------------------------------

Once the block is identified, the local inode index for the local inode table can be identified using:
-----------------------------------------------------
local inode index = (inode - 1) % s_inodes_per_group
-----------------------------------------------------




************* [SAMPLE INODE COMPUTATIONS] **************
----------------|-------------------|-------------------
Inode Number    | BLOCK-GROUP-NUMBER| LOCAL-INODE-INDEX
----------------|-------------------|-------------------
1               | 0                 | 0
963             | 0                 | 962
1712            | 0                 | 1711
1713            | 1                 | 0
3424            | 1                 | 1711
3425            | 2                 | 0
----------------|-------------------|-------------------

s_inodes_per_group = 1712

BLOCK-GROUP-NUMBER = ( (inode - 1) / s_inodes_per_group );

LOCAL-INODE-INDEX  = ( (inode - 1) % s_inodes_per_group );


As many of you are most likely already familiar with, an index of [0] means the first entry. 
The reason behind using [0] rather than [1] is that it can more easily be multiplied by the 
structure size to find the final byte offset of its location in memory or on disk.



