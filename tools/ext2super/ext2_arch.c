#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <linux/ext2_fs.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <string.h>

// ====================================================================

#define PART_NAME "/m-sys/prog-proj/OS-dev/dev/os/256M_fs.ext2"

struct ext2_dir_entry ext2_dir;
struct ext2_super_block mSB;
struct ext2_group_desc mGD;
struct ext2_inode i_node_root;

unsigned char buff[4096];   // информационный буфер

int indev; // дескриптор файла устройства

int BLKSIZE; // размер блока файловой системы

// Определим несколько функций, которые нам понадобятся для работы:

// ====================================================================
// Функция чтения суперблока:
void read_sb() {

    memset(&mSB, 0, 1024);

    //Смещаемся на 1024 байта от начала раздела и считываем суперблок в структуру struct ext2_super_block:
    if (lseek(indev, 1024, 0) < 0) {
        perror("lseek");
        exit(-1);

    }

    if (read( indev,(char *)&mSB, sizeof(mSB) ) < 0) {
        perror("read");
        exit(-1);

    }

    //Проверяем идентификатор файловой системы:
    if (mSB.s_magic != EXT2_SUPER_MAGIC) {
        printf("Неизвестный тип файловой системы! ");
        exit(-1);

    }

    //Значение EXT2_SUPER_MAGIC определено в заголовочном файле .
    //Отображаем информацию о файловой системе, которая находится в суперблоке:

    printf(" Superblock info ----------- \n");
    printf("\tInodes count        : %u \n", mSB.s_inodes_count);
    printf("\tBlocks count        : %u \n", mSB.s_blocks_count);
    printf("\tBlock size          : %u \n", 1024 << mSB.s_log_block_size);
    printf("\tFirst inode         : %d \n", mSB.s_first_ino);
    printf("\tMagic               : 0x%X \n", mSB.s_magic);
    printf("\tInode size          : %d \n", mSB.s_inode_size);
    printf("\tInodes per group    : %u \n", mSB.s_inodes_per_group);
    printf("\tBlosks per group    : %u \n", mSB.s_blocks_per_group);
    printf("\tFirst data block    : %u \n", mSB.s_first_data_block);
    return;

}

// ====================================================================
//Функция чтения таблицы дескрипторов групп:
void read_gdt() {

    printf("----------------------------------------------------\n");
    //Вычисляем размер блока файловой системы:
    BLKSIZE = 1024 << mSB.s_log_block_size;

    printf("\tGroup descriptor table size    : [%d] \n", BLKSIZE);

    // Таблица дескрипторов групп находится в блоке, который расположен сразу же
    // за первым блоком данных (за суперблоком).
    // Считываем таблицу:

    if (lseek(indev, (mSB.s_first_data_block + 1) * BLKSIZE, 0) < 0) {
        perror("lseek");
        exit(-1);

    }

    char BUFF[4096];

    if (read(indev, BUFF, BLKSIZE) < 0) {
        perror("read");
        exit(-1);

    }


    mGD = *((struct ext2_group_desc *) BUFF);

}

// ====================================================================
// Функция получения содержимого inode по его номеру:
void get_inode(int inode_num, struct ext2_inode *i_node) {

    // Входные параметры функции – порядковый номер inode и структура struct ext2_inode.

    __u64 group, index, pos;

    // Вычисляем номер группы блоков, в которой находится inode с порядковым номером inode_num:
    group = (inode_num - 1) / mSB.s_inodes_per_group;

    // Вычисляем позицию inode c порядковым номером inode_num в таблице inode группы
    // group и считываем этот inode в структуру struct ext2_inode:

    index = (inode_num - 1) % mSB.s_inodes_per_group;
    pos = ((__u64)mGD.bg_inode_table) * BLKSIZE + (index * mSB.s_inode_size);
    pread64(indev, i_node, mSB.s_inode_size, pos);


}

// ====================================================================
// Функция чтения блока данных:
void read_iblock(struct ext2_inode *i_node, int blk_num) {

    __u64 pos;

    // Входные параметры функции – структура inode и номер блока (имеется в виду
    // номер из последовательности адресных блоков, расположенных в inode).
    // Вычисляем смещение к информационному блоку на разделе и считываем этот блок в глобальный буфер buff:

    pos = ((__u64) i_node->i_block[ blk_num ]) * BLKSIZE;

    // В буфере buff будет находиться содержимое корневого каталога.
    pread64(indev, buff, BLKSIZE, pos);


}

// ====================================================================
// Функция получения содержимого корневого каталога:
void get_root_dentry() {

    // Порядковый номер inode корневого каталога известен, поэтому получаем содержимое
    // inode корневого каталога и считываем в буфер buff его содержимое:
    get_inode(EXT2_ROOT_INO, &i_node_root );

    read_iblock(&i_node_root, 0);
    // В буфере buff будет находиться содержимое корневого каталога.


}


// ====================================================================
// Функция получения номера inode по имени файла:
int get_i_num(char *name) {

    // Входные параметры функции – имя файла. Возвращаемое значение – порядковый номер inode файла.
    int i = 0, rec_len = 0;

    struct ext2_dir_entry ext2_dir;

    // В буфере buff находится массив записей каталога. Для определения порядкового
    // номера inode файла необходимо найти в этом массиве запись с именем этого файла:

    for (; i < 700; i++) {

        memcpy((void *)&ext2_dir, (buff + rec_len), sizeof(ext2_dir));

        if (!memcmp(ext2_dir.name, name, ext2_dir.name_len)) break;

        rec_len += ext2_dir.rec_len;

    }

    return ext2_dir.inode;

}


// ====================================================================
// А теперь распишем главную функцию:
int main() {


    // абсолютное путевое имя файла
    char full_path[] = "/root_0.file";
    char buff1[ EXT2_NAME_LEN +1 ];

    /*
         2 .
         2 ..
      4099 dir_3
      4101 .Trash-0
      4097 dir_4
      2049 exe
      4098 dir_2
      4100 dir_1
        15 file_root_2.list
        12 file_root_1.list
        13 file_root_4.list
        14 file_root_3.list

    */

    // Открываем файл устройства, считываем суперблок и таблицу дескрипторов групп:
    indev = open( PART_NAME, O_RDONLY );

    if (indev < 0) {
        perror("open");
        exit(-1);

    }

    read_sb();

    read_gdt();


    // Получаем содержимое корневого каталога:
    get_root_dentry();




    // Переменные и структуры:
    struct ext2_inode i_node_tmp;


    // Сейчас в буфере buff находятся все записи корневого каталога (если хотите, можете сохранить их
    // в отдельном файле). Теперь, имея записи корневого каталога, мы можем добраться до
    // содержимого файла test.file, используя вышеприведенный алгоритм чтения файла.
    // С этой целью организуем цикл. В теле цикла проведем разбор абсолютного путевого
    // имени файла, выделяя его элементы – подкаталоги (он у нас один, home) и имя искомого
    // файла (test.file). Для каждого элемента определим порядковый номер inode,
    // считаем этот inode и затем получим содержимое нулевого блока (из последовательности
    // адресных блоков, находящихся в inode):

    int i = 1;
    int n, i_num, outf, type;

    while (1) {

        memset(buff1 ,0, sizeof(buff1) );

        for (n = 0 ; n < EXT2_NAME_LEN; n++, i++) {

            buff1[n] = full_path[i];

            if ((buff1[n] == "/") || (buff1[n] == "?")) {
                i++;
                break;

            }

        }

        buff1[n] = "?";

        // Для каждого элемента абсолютного путевого имени файла определяем порядковый номер
        // inode, считываем этот inode в память и затем получаем содержимое нулевого блока:
        i_num = get_i_num(buff1);
        get_inode(i_num, &i_node_tmp);
        read_iblock(&i_node_tmp, 0);

        // Отобразим информацию о файле (имя, порядковый номер inode, размер файла и его тип):
        printf("Inode number        : %u \n",  i_num );
        printf("File name           : %s \n",  buff1 );
        printf("File size           : %u \n", i_node_tmp.i_size );

        // Тип файла определяют старшие четыре бита поля i_mode структуры struct ext2_inode:
        type = ((i_node_tmp.i_mode & 0xF000) >> 12); // (ext2_inode.i_mode >> 12) == 0x4 )

        printf("Type :%d  \n",type);

        switch (type) {

            case (0x04) :
                printf("(каталог)  \n");
                break;

            case (0x08) :
                printf("(обычный файл)  \n");
                break;

            case (0x06) :
                printf("(файл блочного устройства)  \n");
                break;

            case (0x02) :
                printf("(файл символьного устройства)  \n");
                break;

            default:
                printf("(unknown type)  \n");
                break;

        }

        // Проверяем тип файла. Если это обычный файл – прерываем цикл:
        if (type & 0x08) {

            // В буфере buff будет находиться информация, считанная из информационных блоков файла /home/test.file. Запишем эту информацию в файл:
            // outf = open("out", O_CREAT| O_RDWR, 0600);
            // write(outf, buff, sizeof(buff));
            // close(outf);

            printf("---------------------------\n");
            //printf("[%s]\n", buff);

            break;

        }

        //return 0;
    }

    // Выходим:
    close(indev);
    return 0;

}

// На этом рассмотрение логической структуры файловой системы ext2 завершим.