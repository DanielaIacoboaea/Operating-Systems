#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> 
#include <string.h> 

int main(int argc, char * argv[])
{
    int fd = open("f2.txt", O_WRONLY);
    if (fd == -1)
    {
        printf("could not open file");
        exit(1);
    }else{
        printf("(pid: %d)\n", (int)getpid());
        int rc = fork();
        printf("rc: %d\n", rc);
        if (rc < 0)
        {
            printf("error, fork did not succeed\n");
            exit(1);
        }else if (rc == 0){
            int wr_c = write(fd, "Hi, I am the child!\n", strlen("Hi, I am the child!\n"));
            printf("child wrote %d characters\n", wr_c);
            printf("Hello, I am child (pid: %d)\n", (int)getpid());
        }else {
            int wr_p = write(fd, "I am parent!\n", strlen("I am parent!\n"));
            printf("parent wrote %d characters\n", wr_p);
            printf("Hello, I am parent of %d (pid: %d)\n", rc, (int)getpid());
        }
    }
    write(fd, "Random\n", strlen("Random\n"));
    close(fd);
    return 0;
}   