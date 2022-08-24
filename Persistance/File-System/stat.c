#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    char filename[256];
    struct stat file_info;
    if (argc < 2)
    {
        printf("no file provided\n");
        exit(1);
    }else{
        strcpy(filename, argv[1]);
        stat(filename, &file_info);
        printf("file: %s, inode: %ld, size: %ld, blocks: %ld, Links: %ld\n", filename, file_info.st_ino, file_info.st_size, file_info.st_blocks, file_info.st_nlink);
    }
    return 0;
}
