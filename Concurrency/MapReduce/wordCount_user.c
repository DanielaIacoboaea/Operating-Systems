#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mapreduce.h"
#include <unistd.h>

void* Map(char *file_name) {
    FILE *fp = fopen(file_name, "r");
   // printf("trying to open file: %s\n", file_name);
    assert(fp != NULL);

    char *line = NULL;
    size_t size = 0;
    while (getline(&line, &size, fp) != -1) {
        char *token, *dummy = line;
        while ((token = strsep(&dummy, " \t\n\r")) != NULL) {
            MR_Emit(token, "1");
        }
    }
    free(line);
    fclose(fp);
    return 0;
}

void Reduce(char *key, Getter get_next, int partition_number) {
    int count = 0;
    char *value;
    value = get_next(key, partition_number);
    
    while (value != NULL){
        //printf("value: %s, key: %s, partition number: %d\n", value, key, partition_number);
        count++;
        value = get_next(key, partition_number);
    }
    if (count != 0){
        printf("%s %d\n", key, count);
    }
    //printf("%s %d\n", key, count);
}

int main(int argc, char *argv[]) {
   // printf("main thread pid: %d\n", getpid());
    MR_Run(argc, argv, Map, 10, Reduce, 10, MR_DefaultHashPartition);

    return 0;
}