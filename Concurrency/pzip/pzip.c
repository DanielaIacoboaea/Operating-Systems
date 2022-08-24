#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/sysinfo.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include "common_threads.h"

//the compressed file needs to have
//4-byte integer in binary format followed by the single character in ASCII. 

// split the work between nprocs threads
// if nprocs > the size of the file, limit num of threads to 2
// each thread saves the zipped letters and count in a struct 
// main thread concatenates each threads result in an array 
// and prints the results 

// define struct in which each thread will save the compression
typedef struct pzip_letters{
    char letter;
    int count;
} pzip_letters;


// define struct in which to save pointer to 
// part of the file the thread needs to work on + block size 
typedef struct pzip_arg{
    char* ptr_start;
    int block_size;
} pzip_arg;


typedef struct pzip_thread_ret{
    pzip_letters* zipped_letters;
    int size;
} pzip_thread_ret;


// each thread compares current char to prev char and 
// updates count of repeating ones or
// creates entries for new char/count
void* pzip_thread(void* arg){
    pzip_arg* args = (pzip_arg *) arg;
    pzip_letters* pzip_chars = malloc(1000* sizeof(pzip_letters));
    pzip_thread_ret* r = malloc(sizeof(pzip_thread_ret));

    int size = 0;
    char* get_start = args->ptr_start;
    pzip_chars[size].letter = *get_start;
    pzip_chars[size].count = 1;


    for (int i = 1; i < args->block_size; i++){

        if (*(get_start + i) == *(get_start + (i-1))){
            pzip_chars[size].count++;

        }else{
            size++;
            pzip_chars[size].letter = *(get_start + i);
            pzip_chars[size].count = 1;
        }
    }
   
    r->zipped_letters = pzip_chars;
    r->size = size;
    
    return (void*) r;
}


int main(int argc, char *argv[])
{
    // find nprocs available to create the same number of threads 
    int nprocs_available = get_nprocs();
    
    struct stat file_size;
    int file_size_err;
    int split_file_size;
    int unmmap_err;
    
    // define array in which we'll concatenate all
    // the values returned from threads 
    pzip_letters pzip_threads_concat[2000];
    int pzip_size = 0;
    int start_print_idx;

    pthread_t p[nprocs_available];
    pzip_arg params[nprocs_available];

    pzip_thread_ret* thread_ret;

    //save the filenames from user input in an array
    char filename_input[256];

    //exit if the user does not provide any files 
    if (argc < 2)
    {
        printf("pzip: file1 [file2 ...]\n");
        exit(1);
    }else {
        //loop through all the files and print the bytes output to stdout
        //user input uses > to redirect to a ziped file

        for (int i = 1; i < argc; i++)
        {
            strcpy(filename_input, argv[i]);
            
            int fd = open(filename_input, O_RDWR);
            if(fd < 0){
                printf("pzip: cannot open file\n");
                exit(1);
            }
            
            // get the size of the file
            file_size_err = fstat(fd, &file_size);
            if(file_size_err < 0){
                printf("pzip: can't read file size\n");
                exit(1);
            }

            // split the work between threads
            // based on the file size 
            int last_block_size;
            int threads_num;

            if (file_size.st_size >= nprocs_available){
                threads_num = nprocs_available;
            }else{
                threads_num = 2;
            }

            split_file_size = file_size.st_size/threads_num;
            printf("file_size: %d\n", split_file_size);

            // adjust the last block size
            // depending on the number of threads created
            // to cover edge cases
            if (split_file_size * threads_num < file_size.st_size){
                last_block_size = split_file_size + (file_size.st_size - split_file_size * threads_num);
            }else if (split_file_size * threads_num > file_size.st_size){
                last_block_size = split_file_size - (split_file_size * threads_num - file_size.st_size);
            }else {
                last_block_size = split_file_size;
            }

            // get pointer to the start of the file 
            char *file_ptr = mmap(NULL, file_size.st_size, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
            if(file_ptr == MAP_FAILED){
                printf("Split Error MAP_FAILED\n");
                printf("Error: %s\n", strerror(errno));
                exit(1);
            }
            
            // create threads
            for (int i = 0; i < threads_num; i++){
                params[i].ptr_start = file_ptr + (i * split_file_size);
                params[i].block_size = split_file_size;
                if (i == threads_num - 1){
                    params[i].ptr_start = file_ptr + (i * split_file_size);
                    params[i].block_size = last_block_size;
                }

                Pthread_create(&p[i], NULL, pzip_thread, &params[i]);
            }

            for (int i = 0; i < threads_num; i++){
                Pthread_join(p[i], (void**) &thread_ret);
                pzip_letters* zp = thread_ret->zipped_letters;

                // check if first letter from current thread is the same
                // as the last letter from the prev thread
                // adjust count and starting index
                if (pzip_size != 0){
                    
                    if (zp[0].letter == pzip_threads_concat[pzip_size-1].letter){
                        pzip_threads_concat[pzip_size-1].count += zp[0].count;
                        start_print_idx = 1;
                    }else{
                        start_print_idx = 0;
                    }
                }else{
                    start_print_idx = 0;
                }

                // save zipped letters/count to concatenate array from main
                for (int i = start_print_idx; i <= thread_ret->size; i++){
                    pzip_threads_concat[pzip_size].letter = zp[i].letter;
                    pzip_threads_concat[pzip_size].count = zp[i].count;
                    pzip_size++;
                }
                free(thread_ret);
            }
            close(fd);
            unmmap_err = munmap(file_ptr, file_size.st_size);
            if(unmmap_err != 0){
                printf("Could not unmmap file\n");
                exit(1);
            }
        }
        //printf("\nprinting the array\n");
        for (int i = 0; i < pzip_size; i++){
            printf("%d%c", pzip_threads_concat[i].count, pzip_threads_concat[i].letter);
        }
    }
    return 0;
}
