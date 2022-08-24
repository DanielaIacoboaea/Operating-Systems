

# Concurrency




1. ***C/Linux Projects*** 
    
    - **[Threads](/Concurrency/Threads)** has some pre-written code that needed to be understood and complete each program to achieve various tasks:
    > - The purpose of completing the projects was to understand how a **multi-threaded** is created and how it works, what does a **critical section** mean, **race conditions** and how a non deterministic output can result from **multi-threaded** programs 
    > - How to provide **mutual exclusion** to a critical section using **locks**
    > - How to use **condition variables** to signal between threads
    > - Semaphores
    > - Common concurrency problems 
    > - To compile and run this programs:

            make
            ./foo
    
    - Implement in **[pzip](/Concurrency/pzip)** the simple compression tool based on run-length encoding, **zip**, using *threads* this time to make a parallel version of zip:
    > - Use `get_nprocs()` to create a number of threads to match the number of CPU resources available
    > - Split the file between threads based on the size of the file and the number of threads available
    > - Use `mmap()` to get pointer to the start of the file 
    > - Give each thread a pointer to the part of the file that it needs to access and the block size that it needs to work on 
    > - Each thread saves the compression inside the `struct pzip_letters` that gets returned to the main thread
    > - The main thread concatenates inside an array all the values returned from threads and prints the final result 
     > - To compile and run this programs:

            gcc -o pzip pzip .c -Wall -Werror -pthread -O
            ./pzip file > file.z
    
    - Learn about the general nature of the *MapReduce* paradigm an implement a **[MapReduce](/Concurrency/MapReduce)** library using threads and related functions:
    > - The purpose of the library is to support the execution of user-defined `Map()` and `Reduce()` functions so that users don't have to worry about how to parallelize their application
    > - The user program that we'll work with is **wordCount_user.c** 
    > - The **MapReduce** library needs to handle thread creation and allow many mappers and reducers to run at the same time 
    > - The purpose of the project was implementing `MR_Emit()` and `MR_Run()` functions
    > - The user program calls `MR_Run()` function(giving as parameters the Map() and Reduce() functions to run, number of mapper threads to create and number of reducer threads to create and a partition)
    > - Inside the **mapreduce.c** we have defined an array as a global data structure that holds all the **partitions** that will be created and will store all the keys and values received through the mapper.
    > - Once the computation is done, the Reducer threads start running the return the result for each key (in this case the number of times a word appears in a text)
       > - To compile and run this programs:

            gcc mapreduce.c -o mapreduce -Wall -pthread
            gcc user_program.c mapreduce.c -o user_program -Wall -pthread
            ./user_program file.txt
    

2. ***xv6 Kernel Projects***

    - **[xv6 Lottery Scheduler](/Concurrency/xv6-Lottery-Scheduler)**
    - *A bit of background first:* 
    > - *A sheduling policy* is an algorithm through which, given a number of *RUNNABLE* processes, the OS decides which process should run next 
    > - There are different types of sheduling policies: running process until completion (**FIFO**, **SJF**), run jobs for a time slices, then switch to another job(**Round-Robin**, **MLFQ**)
    > - This project implements a **lottery sheduler**, which has a random approach to sheduling 
    > - Each process receives a number of tickets, the sheduler has information about the total number of tickets, holds a lottery, and the winning process gets to run next 
    > - The processes with more tickets have more chances of winning, so get to be sheduled more often and run more 
    
    - And now let's get through our **lottery sheduler**:
    > - Similar to implementing the `getreadcount()` system call, now we need two other custom system calls: `settickets(int number)` and  `getpinfo(struct pstat *)`
    > - `settickets(int number)` sets the number of tickets of the calling process and `getpinfo(struct pstat *)` returns some information about all running processes, including how many times each has been chosen to run and the process ID of each.
    > - Understand how to use `argint()` to deal with arguments passed into the kernel from user programs
    > - Update the **struct proc** inside **proc.h** with **int tickets**, **int ticks**, **int inuse**
    > - When the process is created assign default number of tickets (1) inside `allocproc()`
    > - Make sure the process inherits the number of tickets from it's parent (inside `fork()`)
    > - A large part of this project was understanding how the xv6 sheduler works(it implements a **Round Robin** policy) by studying the `scheduler()` procedure inside **proc.c** file
    > - The sheduler loops through an array that contains data about all processes **ptable.proc[NPROC]** and checks the state of each process.
    > - When it finds a process with the state *RUNNABLE*, it chooses that process to run, uses *swtch()* to start running that process
    > - What the **lottery sheduler** does:
        > - First compute the total number of tickets available and then generate a random number 
        > - When the sheduler loops through the processes table it has an extra condition to check: any process that has a number of tickets => than the winning ticket is chosen to be sheduled next
         > - To compile and run this program, after [installing xv6](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/INSTALL-xv6.md):

            make qemu-nox
            ./userprogram.c
    
    - **[xv6 Kernel Threads](/Concurrency/xv6-Threads)**
    > - define `clone()` and `join()` system calls and `thread_create()` call, `lock_acquire()` and `lock_release()` functions to support thread creation from user programs 
    > - To define `clone()` we start from `fork()`, much of the information being the same, except that child process shares the same address space as parent process (will share the same **pgdir**)
    > - and is uses the stack passed in as argument, starts running the function passed in and the function has the arguments available as well
    > `join()` system call is very similar with `wait()`, the difference being that `join()` waits for a child thread that shares the address space with the calling process to exit.
    > - To compile and run this program, after [installing xv6](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/INSTALL-xv6.md):

            make qemu-nox
            ./userprogram.c

