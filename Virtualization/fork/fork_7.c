#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
    
    int rc = fork();
    
    if (rc < 0)
    {
        printf("error, fork did not succeed\n");
        exit(1);
    }else if (rc == 0)
    {
        printf("Hello from child (pid: %d)\n", (int)getpid());
        close(STDOUT_FILENO);
        printf("trying to print again after closing stdout");
    }else {
        int rc_wait = wait(NULL);
        printf("Goodby from parent of %d (rc_wait: %d) (pid: %d)\n", rc, rc_wait, (int)getpid());
        
    }
    printf("ouside of blocks\n");
    return 0;
}