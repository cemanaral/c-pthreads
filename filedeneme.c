#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 1024
#define DELIMETERS "?!.,:; \n\t"

int main( int argc, char** argv ){

    const char *fileName = "/home/cem/Desktop/c-pthreads/textfiles/textfile1.txt";
    FILE *textFile = fopen(fileName, "r");
    char buffer[BUFFER_SIZE];
    char *word;

    // Read each line into the buffer
    while(fgets(buffer, BUFFER_SIZE, textFile) != NULL){
        // Gets each token as a string and prints it
        word = strtok(buffer, DELIMETERS);
        while( word != NULL ){
            printf( "-%s-\n", word );
            word = strtok(NULL, DELIMETERS);
        }
    }
    fclose( textFile );
    return 0;
}