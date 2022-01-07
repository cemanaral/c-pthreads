#ifndef MAIN_H_
#define MAIN_H_
void validateArguments(int argc);
char * extractDirectoryFromArguments(char** argv);
int extractNumberOfThreadsFromArguments(char** argv);
void addTextFileNamesToQueue(char * directory);
int doubleSizeArrayOfWords();
int findWord(char* wordToBeFound);
void* worker(void * args);
int main(int argc, char** argv);
#endif