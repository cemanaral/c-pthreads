#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "queue.h"
#include "main.h"

#define DIRECTORY_INDEX 2
#define THREAD_NO_INDEX 4

struct queue* queue;

void validateArguments(int argc) {
    if (argc != 5) {
        fprintf(stderr, "ERROR: Invalid arguments\n");
        fprintf(stderr, "USAGE: ./main.out -d <directoryName> -n <#ofThreads>\n");
        exit(1);
    }
}

char * extractDirectoryFromArguments(char** argv) {
    return argv[DIRECTORY_INDEX];
}

int extractNumberOfThreadsFromArguments(char** argv) {
    return atoi(argv[THREAD_NO_INDEX]);
}

void addTextFileNamesToQueue(char * directory) {
   DIR *dir;
   struct dirent *dirEntry;

   dir = opendir(directory);
   // loops everything under the directory
   while ((dirEntry = readdir(dir)) != NULL) {
       int isFile = (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0);
       // to avoid "." and ".."
       if (isFile)
           queueAdd(queue, dirEntry->d_name);
   }

}

int main(int argc, char** argv) {
    validateArguments(argc);    
    char * directory = extractDirectoryFromArguments(argv);
    int noOfThreads = extractNumberOfThreadsFromArguments(argv);
    
    queue = createQueue();
    addTextFileNamesToQueue(directory);



    return 0;
}