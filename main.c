#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>

#include "queue.h"
#include "main.h"

#define DIRECTORY_INDEX 2
#define THREAD_NO_INDEX 4


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

int main(int argc, char** argv) {
    validateArguments(argc);    
    char * directory = extractDirectoryFromArguments(argv);
    int noOfThreads = extractNumberOfThreadsFromArguments(argv);
    


    return 0;
}