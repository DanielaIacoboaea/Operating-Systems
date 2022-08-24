#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>


void function(char* filename)
{
    printf("filename: %s\n", filename);
}
int main(int argc, char *argv[])
{
   
    const char delimiter[] = ">";
    char input_command[64];
    char *p_input = input_command;
    size_t input_size = 64;
    size_t chars;
    char error_message[30] = "An error has occurred\n";
    printf("wish> ");
    chars = getline(&p_input, &input_size, stdin);
    char *command = NULL;
    char *next_command = NULL;
    char *p_input_command = p_input;
    command = strsep(&p_input_command, delimiter);
    next_command = strsep(&p_input_command, delimiter);
    printf("command: %s\n", command);
    printf("next command: %s\n", next_command);
   
    char file[256];
    strcpy(file, argv[1]);
    printf("file name: %s\n", file);
    function(command);

    int stdout_copy = dup(1);
    close(1);
    int fd = open(file, O_CREAT|O_WRONLY|O_TRUNC,  S_IRWXU);
    printf("fd: %d, command: %s", fd, p_input);
    printf("%s", error_message);
    function(command);
    close(fd);
    dup2(stdout_copy, 1);
    printf("now we can print\n");
    
    if(access("/usr/bin/ls", X_OK))
    {
        printf("exists\n");
    }
    
    return 0;
}