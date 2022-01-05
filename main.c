#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

#include "queue.h"
#include "main.h"

#define DIRECTORY_INDEX 2
#define THREAD_NO_INDEX 4
#define BUFFER_SIZE 1024
#define DELIMETERS "-?!.,:; \n\t"

struct wordInfo {
    char* word;
    char* filename;
};

char * directory;
struct wordInfo *arrayOfWords;
int sizeArrayOfWords = 0;
int arrayOfWordsIndex = 0;
int wordCount = 0;

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


// Doubles the size of arrayOfWords
// returns new size
int doubleSizeArrayOfWords() {
    // if it was not initialized
    // initializes it to hold 8 elements
    if (sizeArrayOfWords==0) {
        sizeArrayOfWords = 8;
        arrayOfWords = malloc(sizeof(struct wordInfo) * sizeArrayOfWords);
    }
    // if was initialized
    else {
        sizeArrayOfWords *= 2;
        arrayOfWords = realloc(arrayOfWords, sizeof(struct wordInfo) * sizeArrayOfWords);
    }
    return sizeArrayOfWords;
}

void worker() {
    char * fileName;
    char * filePath = malloc(150);
    struct wordInfo * currentWordInfo = NULL;
    while (1) {
        // acquire queue lock
        fileName = queueRemove(queue);
        // release queue lock
        if (fileName == NULL) {
            free(filePath);
            return;
        }
            

       
        strcat(filePath, directory);
        strcat(filePath, "/");
        strcat(filePath, fileName);

        
        FILE *textFile = fopen(filePath, "r");
        char buffer[BUFFER_SIZE];
        char *word;

        while(fgets(buffer, BUFFER_SIZE, textFile) != NULL){
            word = strtok(buffer, DELIMETERS);
            while( word != NULL ){
                if (arrayOfWordsIndex >= sizeArrayOfWords) {
                    // acquire array lock
                    printf("doubled array elements size to %d\n", doubleSizeArrayOfWords());
                    // release array lock
                }
                // printf( "-%s-\n", word );

                // acquire array lock
                arrayOfWords[arrayOfWordsIndex].filename = strdup(fileName);
                arrayOfWords[arrayOfWordsIndex].word = strdup(word);
                printf("%s\n", word);
                // release array lock

                // acquire index lock
                arrayOfWordsIndex++;
                // release index lock


                word = strtok(NULL, DELIMETERS);
            }
        }
        fclose(textFile);
        strcpy(filePath, "");
    }
    
}

int main(int argc, char** argv) {
    validateArguments(argc);    
    directory = extractDirectoryFromArguments(argv);
    int noOfThreads = extractNumberOfThreadsFromArguments(argv);
    
    queue = createQueue();
    addTextFileNamesToQueue(directory);
    
    worker();

    printf("--word is %s\n", arrayOfWords[3000].word);
    printf("last index is %d\n", arrayOfWordsIndex);
    return 0;
}