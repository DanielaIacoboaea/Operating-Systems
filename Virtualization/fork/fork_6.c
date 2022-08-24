#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char * argv[])
{
    int status;
    int rc = fork();
    
    if (rc < 0)
    {
        printf("error, fork did not succeed\n");
        exit(1);
    }else if (rc == 0)
    {
        printf("Hello from child (pid: %d)\n", (int)getpid());
        
    }else {
        int rc_wait = waitpid(rc, &status, 0);
        printf("status: %d\n", status);
        printf("Goodby from parent of %d (rc_wait: %d) (pid: %d)\n", rc, rc_wait, (int)getpid());
    }
    return 0;
}