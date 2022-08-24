# Operating Systems
#### Three Easy Pieces 


This repository holds a number of projects used to develop a better understanding of how an Operating System works,
based on the book and online course **Operating Systems**, *Three Easy Pieces*.

Types of projects inside **[Virtualization](/Virtualization)**, **[Concurrency](/Concurrency)** and **[Persistance](/Persistance)** are: 

 - C/Linux Projects
 - xv6 Kernel Projects, using this [install notes](https://github.com/remzi-arpacidusseau/ostep-projects/blob/master/INSTALL-xv6.md) to set it up
 - Each chapter has simulators which mimic some aspects of an operating system

Each **piece** from the course helped me understand what an OS actually does and how it allows many programs to run by managing physical resources (such as the processor, memory, disk) and creating the illusion that each running program has it's own private resource, even though the resources are shared at all times.

The OS takes physical resources and *virtualizes* them, handles issues related to *concurrency* and stores files to *persistent* storing devices.

---

Here we have a short **summary** of the knowledge that projects and simulators help develop throughout the course:

## **[Virtualization](/Virtualization)**



The OS creates this illusion by virtualizing the **CPU** and **Memory**. 
By using *time sharing* and running one process, then stopping it and then running another, it seems like many virtual CPUs exist that run the processess at the same time, when in fact there is a limited number of physical CPUs and the OS uses a *sheduling policy* to decide which program should run next.

On the other hand, by using *space sharing*, the OS divides the resource among the processes that need to use it and each process gets a different part of the resource, for example a block from the disk or a part of memory.
Each process has its own private address space which the OS maps to the physical memory.



## **[Concurrency](/Concurrency)**




Concurrency allows for creation of multi-threaded processes, basically having multiple computations happening at the same time within the same program.

Each thread is like a separate process except that they share the same address space and can access the same data, 
The difference being that each thread has its own stack, a running program with n threads has n stacks within the same address space. The OS can switch running from one thread to another thread similar to switching between processes.


## **[Persistance](/Persistance)** 



Unlike CPU and memory, the OS does not create the illusion of a private and virtualized disk for each process and uses the *file system* to manage the permanent storage devices (disk, RAID's, SSDs) and store any files the user creates on the disks.
