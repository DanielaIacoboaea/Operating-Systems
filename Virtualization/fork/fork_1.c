#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char * argv[])
{
    int x = 100;
    printf("(pid: %d)\n", (int)getpid());
    int rc = fork();
    printf("rc: %d\n", rc);
    if (rc < 0)
    {
        printf("error, fork did not succeed\n");
        exit(1);
    }else if (rc == 0)
    {
        x = 50;
        printf("x in child process: %d\n", x);
        printf("Hello, I am child (pid: %d)\n", (int)getpid());
    }else {
        x = 7;
        printf("x in parent process: %d\n", x);
        printf("Hello, I am parent of %d (pid: %d)\n", rc, (int)getpid());
    }
    return 0;
}