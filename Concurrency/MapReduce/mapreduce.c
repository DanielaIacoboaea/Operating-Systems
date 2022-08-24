#include <stdio.h>
#include "mapreduce.h"
#include "common_threads.h"
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


// define a global data structure to hold all key-value pairs inside partitions 
// inside the global partitions array, each entry represents a single partition 
// the number of partitions is equal to the number of reducers received from the user-program
// each reducer will work on a single partition
// mappers can work on multiple partitions
// each partition struct has the total number of keys available in that partition
// and an allMappings array that holds for each key the mapping between the key and 
// a pointer to the head of a linked list with all values for that key 
// the global data structure is updated concurrently  

// linked list with all values for a single key 
typedef struct values{
    char* value;
    struct values* next;
} values;

// mapping between one key and it's list of values 
typedef struct mapping{
    char *key;
    values* valuesListHead;
    pthread_mutex_t mutex;
} mapping;


// one partition with total number of keys 
// and an array of mapings between each key and all the values associated with that key
typedef struct partition{
   int keys;
   mapping allMappings[100];
   pthread_mutex_t mutexKeys;
} partition;

// args struct that will be passed inside Reducer threads 
typedef struct partitionIdx{
    int i;
} partitionIdx;

// args struct that will be passed inside Mapper threads 
typedef struct mapArgs{
    char* filename;
} mapArgs;

int mappersDone = 0;
int partitionsCount;
partition partitions[50];
int filesCount = 0;
Partitioner partitionReceived;
Reducer reducerReceived;
Mapper mapperReceived;

pthread_mutex_t countLock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t fillPartitions = PTHREAD_MUTEX_INITIALIZER;

pthread_cond_t partitionsFilled   = PTHREAD_COND_INITIALIZER;

// hash function decides in which partition each key will go 
unsigned long MR_DefaultHashPartition(char *key, int num_partitions) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++) != '\0')
        hash = hash * 33 + c;
    return hash % num_partitions;
}


// find the index of the key that we're looking inside the allMappings array
int wrapperGet_nextKeyIdx(char* key, int partition_number){
    
    for (int i = 0; i < partitions[partition_number].keys; i++){
        if(strcmp(key, partitions[partition_number].allMappings[i].key) == 0){
            return i;
        }
    }
    return -1;
}

// loop through the list values for one key and return the value
// until we reach the end of the list
char* get_next(char* key, int partition_number){
    
    int keyIdx = wrapperGet_nextKeyIdx(key, partition_number);
    if (keyIdx == -1){
        return NULL;
    }else{
        pthread_mutex_lock(&partitions[partition_number].allMappings[keyIdx].mutex);
        values* currentValue = partitions[partition_number].allMappings[keyIdx].valuesListHead;
        if(currentValue){
            char *returnValue = currentValue->value;
            partitions[partition_number].allMappings[keyIdx].valuesListHead = currentValue->next;
            free(currentValue);
            pthread_mutex_unlock(&partitions[partition_number].allMappings[keyIdx].mutex);
            return returnValue;
        }else{
            pthread_mutex_unlock(&partitions[partition_number].allMappings[keyIdx].mutex);
            return NULL;
        }
    }
}

// add each key-value pair inside it's partition
// if the partition is empty (keys == 0), create the first key
// if partition has other keys, and the urrent key does not exist, create it
// if the key already exists, update the values list for that key 
void MR_Emit(char *key, char *value){

    unsigned long partitionNum = MR_DefaultHashPartition(key, partitionsCount);

    pthread_mutex_lock(&partitions[partitionNum].mutexKeys);
    if (key){

        int keyExists = 0;
        if (partitions[partitionNum].keys != 0){
            pthread_mutex_unlock(&partitions[partitionNum].mutexKeys);
            
            for(int i = 0; i < partitions[partitionNum].keys; i++){
                if (strcmp(partitions[partitionNum].allMappings[i].key, key) == 0){
                    keyExists = 1;
                    values* newValue = malloc(sizeof(values));
                    pthread_mutex_lock(&partitions[partitionNum].allMappings[i].mutex);
                    newValue->value = value;
                    newValue->next = partitions[partitionNum].allMappings[i].valuesListHead;
                    partitions[partitionNum].allMappings[i].valuesListHead = newValue;
                    pthread_mutex_unlock(&partitions[partitionNum].allMappings[i].mutex);
                    break;
                }
            }
            
        }else if (partitions[partitionNum].keys == 0){
            partitions[partitionNum].allMappings[0].key = key;
            keyExists = 1;
            values* newValue = malloc(sizeof(values));
            if (newValue == NULL){
                printf("new value in MR EMIT failed\n");
            }
            pthread_mutex_lock(&partitions[partitionNum].allMappings[0].mutex);
            newValue->value = value;
            newValue->next = NULL;
            partitions[partitionNum].allMappings[0].valuesListHead = newValue;
            partitions[partitionNum].keys++;
            pthread_mutex_unlock(&partitions[partitionNum].mutexKeys);
            pthread_mutex_unlock(&partitions[partitionNum].allMappings[0].mutex);
        }

        if (keyExists == 0){
            partitions[partitionNum].keys++;
            pthread_mutex_unlock(&partitions[partitionNum].mutexKeys);
            partitions[partitionNum].allMappings[partitions[partitionNum].keys-1].key = key;
            values* newValue = malloc(sizeof(values));
            if (newValue == NULL){
                printf("new value in MR EMIT failed\n");
            }
            pthread_mutex_lock(&partitions[partitionNum].allMappings[0].mutex);
            newValue->value = value;
            newValue->next = NULL;
            partitions[partitionNum].allMappings[partitions[partitionNum].keys-1].valuesListHead = newValue;
            pthread_mutex_unlock(&partitions[partitionNum].allMappings[0].mutex);
        }
    }
}


void* wrapperMapper(void* args){
    pthread_mutex_lock(&fillPartitions);
    pthread_mutex_lock(&countLock);
        mapArgs *filename = (mapArgs *)args;
    pthread_mutex_unlock(&countLock);

    mapperReceived(filename->filename);
    return 0;
}

void* wrapperReducer(void* i){
    
    partitionIdx *partitionNum = (partitionIdx *)i;

    pthread_mutex_lock(&partitions[partitionNum->i].mutexKeys);
    int keysNumInPartition = partitions[partitionNum->i].keys;
    pthread_mutex_unlock(&partitions[partitionNum->i].mutexKeys);

    for (int i = 0; i < keysNumInPartition; i++){
        reducerReceived(partitions[partitionNum->i].allMappings[i].key, get_next, partitionNum->i);
    }
    return NULL;
}


// initialize the global data structure 
// create all mapper threads 
// create all reducer threads after the mapper threads finished execution
void MR_Run(int argc, char *argv[], Mapper map, int num_mappers, Reducer reduce, int num_reducers, Partitioner partition){
   
    pthread_t mappers[num_mappers];
    pthread_t reducers[num_reducers];
    partitionsCount = num_reducers;
    partitionReceived = partition;
    reducerReceived = reduce;
    mapperReceived = map;

    int numFiles = argc - 1;

    pthread_mutex_lock(&fillPartitions);
    for (int i = 0; i < num_reducers; i++){
        partitions[i].keys = 0;
        partitions[i].allMappings[0].valuesListHead = NULL;
        pthread_mutex_init(&partitions[i].allMappings[0].mutex, NULL);
        pthread_mutex_init(&partitions[i].mutexKeys, NULL);
    }
    pthread_mutex_unlock(&fillPartitions);
    mapArgs argsMapper;

    // start mapper threads 
    //pthread_mutex_lock(&fillPartitions);
    do{
        int j = 0;
        while (j < num_mappers && filesCount < numFiles){
            pthread_mutex_lock(&countLock);
            filesCount++;
            argsMapper.filename = argv[filesCount];
            pthread_mutex_unlock(&countLock);
            Pthread_create(&mappers[j], NULL, wrapperMapper, &argsMapper);
            j++;
        }
        for (int i = 0; i < j; i++){
            Pthread_join(mappers[i], NULL);
        }
    }while (filesCount < numFiles);
    mappersDone = 1;
    pthread_cond_signal(&partitionsFilled);
    pthread_mutex_unlock(&fillPartitions);

    // start reducer threads
    partitionIdx argsReducer;

    for (int i = 0; i < partitionsCount; i++){
        pthread_mutex_lock(&fillPartitions);
        while (mappersDone == 0){
            pthread_cond_wait(&partitionsFilled, &fillPartitions);
        }
        pthread_mutex_unlock(&fillPartitions);
        
        if(partitions[i].keys != 0){
            argsReducer.i = i;
            Pthread_create(&reducers[i], NULL, wrapperReducer, &argsReducer);
        }
    }

    for (int i = 0; i < partitionsCount; i++){
         if(partitions[i].keys != 0){
            Pthread_join(reducers[i], NULL);
        }
    }
   
    for (int i = 0; i < partitionsCount; i++)
    {
        for (int j = 0; j < partitions[i].keys; j++){
            if (partitions[i].keys != 0){
            free(partitions[i].allMappings[j].valuesListHead);
            }
        }
    }
}
