

# Persistance



1. ***C/Linux Projects*** 
    
    > - This **piece** of the course focused more on simulations to understand how persistance data storage devices work(hard disk, RAID's and SSDs)
    > - Understand what **seek time**, **rotational delay** and **transfer time** mean for reads and writes and how they can affect disk **performance** and influence disk sheduling policies 
    > - Understanding that the **file system** is the software from the **OS** that manages the **persistent storage devices** and help build the abstractions of **files** and  **directories**
    > - The **file system** API provides an interface through which to **create**, **access** and **delete** files on disk, all located under the directory tree
    > - Understanding what type of **on-disk data structures** are used by the file system to organize it's data and metadata, and how to access the data: inodes, data blocks, the superblock, inode bitmap and data bitmap by using simulations
    > - Understand the data structures that need to be updated when reading or writting to disk and locality
    > - Understand how a file system checker or journaling can recover from **crashes** and help keep the on-disk image in a reliable state
    > - The inside **[File Systems](/Persistance/File-Systems)** use `stat()` system call to access data about files like: **type of file**, **reference count**, **inode**, **owner**, **size**, number of **blocks** allocated on the disk, time of last **access/modification** 
    > - To compile and run this programs:

            gcc -o stat stat.c -Wall -Werror
            ./stat file