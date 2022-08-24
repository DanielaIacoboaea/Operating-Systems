#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <dirent.h>

int main(int argc, char *argv[])
{
    struct stat file_info;
    struct dirent *directory;
    DIR *currentDir;
    if (argc == 1){
        currentDir = opendir(".");
        assert(currentDir != NULL);
    }else if (argc == 2 && argv[1] != "-l"){
        currentDir = opendir(argv[1]);
        assert(currentDir != NULL);
    }else if (argc == 3 && argv[2] != "-l"){
        currentDir = opendir(argv[2]);
        assert(currentDir != NULL);
    }
    while((directory == readdir(currentDir)) != NULL){
        printf("%ld, %s\n", (unsigned long)currentDir->d_ino, currentDir->d_name);
        stat(currentDir->d_name, &file_info);
        printf("file: %s, inode: %ld, size: %ld, blocks: %ld, Links: %ld\n", currentDir->d_name, file_info.st_ino, file_info.st_size, file_info.st_blocks, file_info.st_nlink);

    }
    closedir(currentDir);
    return 0;
}
