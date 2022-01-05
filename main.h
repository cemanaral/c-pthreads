#ifndef MAIN_H_
#define MAIN_H_
void validateArguments(int argc);
char * extractDirectoryFromArguments(char** argv);
int extractNumberOfThreadsFromArguments(char** argv);
void initializeQueues(struct queue** queues, int noOfThreads);
#endif