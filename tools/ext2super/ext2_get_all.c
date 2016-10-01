// ext2group.c

// Reads the first (and only) group-descriptor from a Ext2 floppy disk.

// Questions?
// Emanuele Altieri
// ealtieri@hampshire.edu


#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/ext2_fs.h>

#define BLOCK_OFFSET(block) (BASE_OFFSET+(block-1)*block_size)
#define BASE_OFFSET 1024            // locates beginning of the super block (first group)
unsigned int block_size = 0;        // block size (to be calculated)

void read_dir(int, const struct ext2_inode*, const struct ext2_group_desc*);
void read_inode(int, int, const struct ext2_group_desc*, struct ext2_inode*);

// ====================================================================
int main( unsigned int argc, char *argv[] ) {

    // ----------------------------------------------------------------
    if ( argc < 2 ) {
        printf("\nUsage: %s ./path/to/device \n\n", argv[0]); return 1;
    }


    char *dev = argv[1];
    struct ext2_super_block super;
    struct ext2_group_desc group;
    struct ext2_inode inode;


    //printf(" sizeof(ext2_inode) [%d]\n", sizeof(inode) );
    //printf(" sizeof(__le16) [%d]\n", sizeof(__le16) );
    //printf(" sizeof(__le32) [%d]\n", sizeof(__le32) );


    //return 0;

    int ext2;


    if ((ext2 = open(dev, O_RDONLY)) < 0) {
        perror(dev); return 1;
    }

    // read super-block
    lseek(ext2, BASE_OFFSET, SEEK_SET);
    read(ext2, &super, sizeof(super));

    if (super.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, " ['%s']  Not a Ext2 filesystem\n", dev ); return 1;
    }

    //printf(" super.s_magic: [0x%x] [%d] \n", super.s_magic, super.s_magic );
    //return 0;

    // ----------------------------------------------------------------
    block_size = 1024 << super.s_log_block_size;

    // ----------------------------------------------------------------
    /*
    printf("\n-----------------------------------------------------\n");
    printf("\tsuper.s_uuid            : [%s]\n", super.s_uuid );
    printf("[SUPER-BLOCK] from ext2 ['%s']:\n"
           "\tInodes count            : %u\n"
           "\tBlocks count            : %u\n"
           "\tReserved blocks count   : %u\n"
           "\tFree blocks count       : %u\n"
           "\tFree inodes count       : %u\n"
           "\tFirst data block        : %u\n"
           "\tBlock size              : %u\n"
           "\tBlocks per group        : %u\n"
           "\tInodes per group        : %u\n"
           "\tCreator OS              : %u\n"
           "\tFirst non-reserved inode: %u\n"
           "\tSize of inode structure : %hu\n"
           ,
           dev,
           super.s_inodes_count,
           super.s_blocks_count,
           super.s_r_blocks_count,     // reserved blocks count
           super.s_free_blocks_count,
           super.s_free_inodes_count,
           super.s_first_data_block,
           block_size,
           super.s_blocks_per_group,
           super.s_inodes_per_group,
           super.s_creator_os,
           super.s_first_ino,          // first non-reserved inode
           super.s_inode_size);
    */


    //return 0;
    // ----------------------------------------------------------------
    // read group descriptor
    lseek(ext2, BASE_OFFSET + block_size, SEEK_SET);
    read(ext2, &group, sizeof(group));


    /*
    printf("-----------------------------------------------------\n");
    printf("SIZE OF ext2_group_desc[%d]\n", sizeof(group));

    printf("-----------------------------------------------------\n");
    printf("first [GROUP-DESCRIPTOR] from ext2 ['%s']:\n"
           "\tBlocks bitmap block: %u\n"
           "\tInodes bitmap block: %u\n"
           "\tInodes table block : %u\n"
           "\tFree blocks count  : %u\n"
           "\tFree inodes count  : %u\n"
           "\tDirectories count  : %u\n"
           ,
           dev,
           group.bg_block_bitmap,
           group.bg_inode_bitmap,
           group.bg_inode_table,
           group.bg_free_blocks_count,
           group.bg_free_inodes_count,
           group.bg_used_dirs_count);    // directories count
    */
    // ----------------------------------------------------------------
    unsigned int inode_no;
    /*
    printf("\n-----------------------------------------------------\n");
    printf("DIRECTORY : ['/']\n");

        inode_no = 0;

        while ( inode_no < 128 ) {

            printf("----------------------------------------------\n" );
            printf("reading ext2_inode: [%d] -> ['0x%x']\n", inode_no, &inode );
            read_inode(ext2, inode_no++, &group, &inode);   // read inode [2] == [root directory]

            printf("reading ext2_dir: G:[0x%x] ['0x%x']\n", &group, &inode );
            read_dir(ext2, &inode, &group);

        }


    */


    inode_no = 2; // 2 == root

    printf("----------------------------------------------\n" );
    read_inode(ext2, inode_no, &group, &inode);   // read inode [2] == [root directory]

    printf("reading ext2_inode:     [%d] -> ['0x%x']\n", inode_no, &inode );

    printf("inode.i_mode:           [%d]\n", inode.i_mode ); // [I_NODE] File mode */
    printf("inode.i_uid:            [%d]\n", inode.i_uid ); // [I_NODE] Owner Uid */
    printf("inode.i_size:           [%d]\n", inode.i_size ); // [I_NODE] Size in bytes */
    printf("inode.i_atime:          [%d]\n", inode.i_atime ); // [I_NODE] Access time */
    printf("inode.i_ctime:          [%d]\n", inode.i_ctime ); // [I_NODE] Creation time */
    printf("inode.i_mtime:          [%d]\n", inode.i_mtime ); // [I_NODE] Modification time */
    printf("inode.i_dtime:          [%d]\n", inode.i_dtime ); // [I_NODE] Deletion Time */
    printf("inode.i_gid:            [%d]\n", inode.i_gid ); // [I_NODE] Group Id */
    printf("inode.i_links_count:    [%d]\n", inode.i_links_count ); // [I_NODE] Links count */
    printf("inode.i_blocks:         [%d]\n", inode.i_blocks ); // [I_NODE] Blocks count */
    printf("inode.i_flags:          [%d]\n", inode.i_flags ); // [I_NODE] File flags */



    //return 0;

    printf("reading ext2_dir: G:[0x%x] ['0x%x']\n", &group, &inode );
    read_dir(ext2, &inode, &group);

    close(ext2);

    // ----------------------------------------------------------------
    return 0;

}
// ====================================================================
void read_inode(int _ext2, int inode_no, const struct ext2_group_desc *group, struct ext2_inode *inode) {

    /* lseek(
        _ext2,

        // [bg_inode_table] contains the block number of the start of the inode table
        // of the current blocks group. The inode table is just the actual inodes
        // which are reserved for the current block.

        // NO MACRO
        // ( 1024  + (132 - 1) * 1024 ) == 135168
        ( BASE_OFFSET+ (group->bg_inode_table - 1) * block_size )
        +

        // (2 - 1) * 128 == 128
        ("inode_no > 2"  -1) * sizeof(struct ext2_inode)

        // == 135168 * 128  17.301.504

        // == 17.301.504 / 1024 = 16.896 KiB
        // == 17.301.504 / 1024 / 1024 = 16 MiB



        // MACRO
        // BLOCK_OFFSET(group->bg_inode_table)+(inode_no-1)*sizeof(struct ext2_inode)

        , SEEK_SET);

    */


    lseek(
        _ext2,

        // [bg_inode_table] contains the block number of the start of the inode table
        // of the current blocks group. The inode table is just the actual inodes
        // which are reserved for the current block.

        // --------------------------------------------
        // NO MACRO
        // ( 1024  + (132 - 1) * 1024 ) == 135168
        ( BASE_OFFSET+ (group->bg_inode_table - 1) * block_size )
        +
        // (2 - 1) * 128 == 128
        (inode_no - 1) * sizeof(struct ext2_inode)

        // --------------------------------------------
        // MACRO
        // BLOCK_OFFSET(group->bg_inode_table)+(inode_no-1)*sizeof(struct ext2_inode)
        // --------------------------------------------

        , SEEK_SET);


    read(
        _ext2, inode, sizeof(struct ext2_inode));

}

// ====================================================================
void read_dir(int _ext2, const struct ext2_inode *inode, const struct ext2_group_desc *group) {


    //printf("inode->i_mode: [%s] [%d] [0x%x]\n", inode->i_mode, inode->i_mode, inode->i_mode);
    //printf("inode->i_mode: [%s] [%d] [0x%x]\n", inode->i_mode, inode->i_mode, inode->i_mode);
    printf("inode->i_mode: [0x%X] [%d]\n", inode->i_mode, inode->i_mode );
    //return;

    if ( inode->i_mode <= 0 ) return;

    /* .../__docs__/FS/ext/The%20Second%20Extended%20File%20System.html
    Table 3-13. Inode Structure ( SIZE IN BYTE)
    ----------------------------------------------------------------------
    Offset  Size    Description
    0       2       i_mode
    2       2       i_uid
    4       4       i_size
    8       4       i_atime
    12      4       i_ctime
    16      4       i_mtime
    20      4       i_dtime
    24      2       i_gid
    26      2       i_links_count
    28      4       i_blocks
    32      4       i_flags
    36      4       i_osd1
    40      15 x 4  i_block
    100     4       i_generation
    104     4       i_file_acl
    108     4       i_dir_acl
    112     4       i_faddr
    116     12      i_osd2
    ----------------------------------------------------------------------
    */


    /* 16bit value used to indicate the format of the described file and the access rights.
    Here are the possible values, which can be combined in various ways:

    Table 3-15. Defined [i_mode] Values [3.5.1.]

    Constant        Value       Description
    ----------------------------------------------------------------------
    -- file format --
    EXT2_S_IFSOCK   0xC000      socket
    EXT2_S_IFLNK    0xA000      symbolic link
    EXT2_S_IFREG    0x8000      regular file
    EXT2_S_IFBLK    0x6000      block device
    EXT2_S_IFDIR    0x4000      directory
    EXT2_S_IFCHR    0x2000      character device
    EXT2_S_IFIFO    0x1000      fifo
    -- process execution user/group override --
    EXT2_S_ISUID    0x0800      Set process User ID
    EXT2_S_ISGID    0x0400      Set process Group ID
    EXT2_S_ISVTX    0x0200      sticky bit
    -- access rights --
    EXT2_S_IRUSR    0x0100      user read
    EXT2_S_IWUSR    0x0080      user write
    EXT2_S_IXUSR    0x0040      user execute
    EXT2_S_IRGRP    0x0020      group read
    EXT2_S_IWGRP    0x0010      group write
    EXT2_S_IXGRP    0x0008      group execute
    EXT2_S_IROTH    0x0004      others read
    EXT2_S_IWOTH    0x0002      others write
    EXT2_S_IXOTH    0x0001      others execute
    ----------------------------------------------------------------------
    */

    void *block;

    if (S_ISDIR(inode->i_mode)) {

        // allocate memory for the data block
        if ((block = malloc(block_size)) == NULL) {
            fprintf(stderr, "Memory error\n");
            close(_ext2);
            exit(1);
        }
        // (block) (BASE_OFFSET+(block-1)*block_size)
        lseek(_ext2, (BASE_OFFSET+ (inode->i_block[0] - 1) * block_size)  , SEEK_SET);


        printf("OFFSET: [%d]\n", (BASE_OFFSET+ (inode->i_block[0] - 1) * block_size) );
        read(_ext2, block, block_size); // read block from disk
        struct ext2_dir_entry_2 *entry; // first entry in the directory
        //printf("sizeof([%d])\n",  sizeof(struct ext2_dir_entry_2));

        entry = (struct ext2_dir_entry_2 *) block;

        // Notice that the list may be terminated with a NULL
        //entry (entry->inode == NULL);

        unsigned int size = 0;

        while ((size < inode->i_size) && entry->inode) {

            char file_name[EXT2_NAME_LEN+1];
            char _name[256];

            //printf("entry->name_len: [%d]\n", entry->name_len);

            unsigned int _i=0;

            while ( _i < entry->name_len ) {
                _name[ _i ] = entry->name[ _i++ ];
                _name[ _i ] = '\0';
            }


            //memcpy(file_name, entry->name, entry->name_len);
            // append null character to the file name
            file_name[entry->name_len] = 0;
            //printf("%10u %s\n", entry->inode, file_name);
            printf("%10u %s\n", entry->inode, _name);
            entry = (void*) entry + entry->rec_len;
            size += entry->rec_len;
        }

        free(block);
    }

}

// ====================================================================

