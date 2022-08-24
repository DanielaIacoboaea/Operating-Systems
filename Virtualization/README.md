

# Virtualization



1. ***C/Linux Projects*** 
    
    - Build simple versions of commonly used commands like cat and grep called **[wcat](/Virtualization/cat)** and **[wgrep](/Virtualization/grep)**
    > - Use `fopen()`, `fgets()`, and `fclose()` to open the files received as command-line arguments and print out their contents 
    > -  Search through a file a user-specified search term in the line. If the line contains the word, print that line 
    > - To compile and run this programs:

        >>    gcc -o wcat wcat.c -Wall -Werror
        >>    ./wcat file.txt
        >>    gcc -o wgrep wgrep.c -Wall -Werror
        >>    ./wgrep foo bar.txt

    > - To run the tests for this programs:
    
        >>    ./test-wcat.sh
        >>    ./test-wgrep.sh
        
    - Build a file compression tool (**[wzip](/Virtualization/zip_project)**) and a file decompression tool(**[wunzip](/Virtualization/zip_project)**)
    > - The type of compression: run-length encoding (RLE). Count the number of the same consecutive characters and turn that into the number and a single instance of the character (e.g aaaaaaaaaabbbb turns into 10a4b).
    > - Use file decompression to read through the file and reverse the process 
    > - To compile and run this programs:
        >>    gcc -o wzip wzip.c -Wall -Werror
        >>    ./wzip file.txt > file.z
        >>    gcc -o wunzip wunzip.c -Wall -Werror
        >>    ./wunzip file.z
    > - To run the tests for this programs:
        >>    ./test-wzip.sh
        >>    ./test-wunzip.sh

    - Understand process creation using [`fork()`](/fork) system call
    > - Gain familiarity with the `fork()` system call which the OS provides as a way to create a new process
    > - Use `wait()` system call to make the output deterministic, delay the parent's execution until the child process is done and `wait()` returns to parent 
    > - Use the `exec()` family of system calls to allow a child process to execute an entirely new program (`ls`) 
    and understand that it transforms the currently running program into a different running program
    > - How does `exec()` manage to do this? 
         >> - When it loads code from the new executable provided (e.g `ls`) and overwrittes it's current code segment, the memory space of the program is re-initialized (stack, heap, static data)
    > - To compile and run this programs:
        >>    gcc -o fork fork.c -Wall -Werror
        >>    ./fork
    
    - Build a user program called **[wish](/Virtualization/wish-shell)**  using `fork()`, `wait()` and `exec()` similiar to a shell that prompts 
    the user for a command, 
    > - calls `fork()` to create a new child process to run the command
    > - calls `execvp()` to run the executable program typed in the command and waits for the command to complete. 
    > - when the child process returns, the shell prompts the user for the next command. 
    > - **wish** has `exit`, `cd`, and `path` as built-in commands
    > - **wish** provides redirection of the standard output if the user provides a filename after the `>` character
    > - the user can change the default paths `path /bin /usr/bin`, if the path is set to empty, only the built-in commands are available
    > - To compile and run this programs:
        >>    gcc -o wish wish.c -Wall -Werror
        >>    ./wish
        >>    ./wish batch.txt

2. ***xv6 Kernel Projects***

    - Add a **[system call](/Virtualization/xv6-System-call)** to xv6 called `getreadcount()`
    - *A bit of background first:* 
    > - The hardware provides different modes of execution: **user mode** and **kernel mode**
    > - The concept of *Limited Direct Execution* allows a user program to run directly on the CPU and thus be fast
    > - At the same time, a user program runs in **user mode** and is restricted in what it can do for security and efficiency reasons (can't issue I/O requests, allocate more memory, etc) 
    > - But still a process must be able to perform I/O and some other restricted operations, but without giving the process complete control over the system
    > - The way it achieves this is through system calls performed by user programs
    > - System calls execute a special **trap** instruction that jumps into the kernel, raises the privilege level to **kernel mode** and now has access to full resource
    > - After executing the privileged operations, it **returns from the trap** instruction and reduces the privilege level back to **user mode**
    
    - And now let's get through our custom `getreadcount()` system call: 
    > - It has to return how many times the `read()` system call was called by user processes since the time that the kernel was booted.
    > - To trace how a system call is implemented, I spent some time understanding the kernel code base and tracing how other system calls work, for e.g: `getpid()`
    > - To keep track of all reads, a global variable **readcounter** is used and incremented every time the `read()` system call is used
    > - In order to access this variable from any process, inside **proc.h** file, where the struct for a process is defined, a **int readcounter** field was added, in order to be updated and returned when the `getreadcount()` is called from a user program
    > - This field is initialized to 0 when the process is created (inside **proc.c** file, `allocproc()` routine) 
    > - to implement `getreadcount()`, first assign a system call number in **syscall.h** which is later used in **syscall.c**, each system call has a specific number assigned to it
    > - In **syscall.c**, that number is used to call the function that needs to run, both the number of the system call and a pointer to that function are added in the array **syscalls**
    > - So the function `sys_getreadcount` will be called
    > - In the `syscall()` function implemented in the **syscall.c** file, we add an extra check: if the current system call made has the same number as the `read()` system call, then increment the global variable accordingly 
    > - If the current system call is `getreadcount()`, then update **readcounter** data inside the current process so that we can return it
    > - Because `sys_getreadcount` function is implemented in **sysproc.c file** we added only the function prototype **extern int sys_getreadcount(void)** here, similar to the other system calls
    > - To make the system call available to user programs, update **user.h** and ** and **usys.S** files, `getreadcount()` being the system call that the user can make
     > - To compile and run this program, after [installing xv6](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/INSTALL-xv6.md):
        >>    make qemu-nox
        >>    ./userprogram.c



