#include "types.h"
#include "stat.h"
#include "user.h"
#include "fcntl.h"

lock_t* tlock;

void thread(void* arg1, void* arg2){

    lock_acquire(tlock);
    printf(1, "enters new thread\n");
    lock_release(tlock);
    exit();
}


int main(int argc, char *argv[])
{
    
    lock_init(tlock);
    int arg1 = 10;
    int arg2 = 20;

    printf(1, "parent thread starts\n");

    thread_create(&thread, (void *)&arg1, (void *)&arg2);
    thread_join();

    printf(1, "parent thread ends\n");
    
    exit();
}