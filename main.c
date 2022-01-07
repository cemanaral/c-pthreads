#include <stdio.h>
#include <stdlib.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>

#include "queue.h"
#include "main.h"

#define DIRECTORY_INDEX 2
#define THREAD_NO_INDEX 4
#define BUFFER_SIZE 1024
#define DELIMETERS "-?!.,:; \n\t"

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t array_mutex = PTHREAD_MUTEX_INITIALIZER;

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

// adds text files to queue
// returns the number of files added
int addTextFileNamesToQueue(char * directory) {
   DIR *dir;
   struct dirent *dirEntry;
   int noOfFiles = 0;

   dir = opendir(directory);
   // loops everything under the directory
   while ((dirEntry = readdir(dir)) != NULL) {
       int isFile = (strcmp(dirEntry->d_name, ".") != 0 && strcmp(dirEntry->d_name, "..") != 0);
       // to avoid "." and ".."
       if (isFile) {
           queueAdd(queue, dirEntry->d_name);
           noOfFiles++;
       }
           
   }
   return noOfFiles;
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

// returns the index of given word
// in arrayOfWords
// if not found,
// returns -1 
int findWord(char* wordToBeFound) {
    char* currentWord; 
    for (int i=0; i < arrayOfWordsIndex; i++) {
        currentWord = arrayOfWords[i].word;
        if (strcmp(wordToBeFound, currentWord)==0) {
            return i;
        }
    }
    return -1;
} 

void* worker(void * args) {
    char * fileName;
    char * filePath = malloc(150);
    struct wordInfo * currentWordInfo = NULL;
    while (1) {
        // acquire queue lock
        pthread_mutex_lock(&queue_mutex);
        fileName = queueRemove(queue);
        printf("thread: %ld has been assigned file %s\n", pthread_self(), fileName);
        // release queue lock
        pthread_mutex_unlock(&queue_mutex);

        if (fileName == NULL) {
            free(filePath);
            pthread_exit(NULL);
        }
            

       
        strcat(filePath, directory);
        strcat(filePath, "/");
        strcat(filePath, fileName);

        
        FILE *textFile = fopen(filePath, "r");
        char buffer[BUFFER_SIZE];
        char *word;
        int index;

        while(fgets(buffer, BUFFER_SIZE, textFile) != NULL){
            char* rest = buffer;
            word = strtok_r(rest, DELIMETERS, &rest);
            while( word != NULL ){
                pthread_mutex_lock(&array_mutex);
                index = findWord(word);
                if (index != -1) {
                     printf("thread: %ld %s already exists at index %d\n", pthread_self(), word, index);
                     word = strtok_r(rest, DELIMETERS, &rest);
                     pthread_mutex_unlock(&array_mutex);
                     continue;
                }
                if (arrayOfWordsIndex >= sizeArrayOfWords) {
                    printf("thread: %ld enlarged array elements size to %d\n", pthread_self(),doubleSizeArrayOfWords());
                }
                arrayOfWords[arrayOfWordsIndex].filename = strdup(fileName);
                arrayOfWords[arrayOfWordsIndex].word = strdup(word);
                printf("thread: %ld added %s to index %d\n", pthread_self(), word, arrayOfWordsIndex);
                arrayOfWordsIndex++;
                word = strtok_r(rest, DELIMETERS, &rest);
                pthread_mutex_unlock(&array_mutex);
                
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
    int noOfFiles = addTextFileNamesToQueue(directory);

    pthread_t threads[noOfThreads];
    for (int i=0; i < noOfThreads; i++) {
        pthread_create(&threads[i], NULL, worker, NULL);
    }

    for (int i=0; i < noOfThreads; i++) {
        pthread_join(threads[i], NULL);
    }

    free(arrayOfWords);
    printf("MAIN THREAD: All done (successfully read %d words with %d threads from %d files).\n",
        arrayOfWordsIndex,
        noOfThreads,
        noOfFiles
    );


    return 0;
}