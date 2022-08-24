#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
    printf("Hello, world (pid: %d)\n", (int)getpid());
    int rc = fork();
    printf("rc: %d\n", rc);
    if (rc < 0)
    {
        printf("error, fork did not succeed\n");
        exit(1);
    }else if (rc == 0)
    {
        printf("Child (pid: %d)\n", (int)getpid());
        execl("/bin/ls", "ls", NULL);
    }else {
        int rc_wait = wait(NULL);
        printf("parent of %d (rc_wait: %d) (pid: %d)\n", rc, rc_wait, (int)getpid());
    }
    return 0;
}