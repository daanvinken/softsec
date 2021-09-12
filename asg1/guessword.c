/*******************************************************************************
* Title                 :   Guessword password cracker MD5 (multithreaded)
* Filename              :   guessword.c
* Author                :   Daan Vinken
* Origin Date           :   12/09/2021
* Version               :   1.0.0
* Notes                 :   None
*******************************************************************************/

/******************************************************************************
* Includes
*******************************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <crypt.h>

/******************************************************************************
* Global definitions
*******************************************************************************/
#define MD5 "$1$M9"
#define SALT "M9"
#define CHUNKSIZE 100

/******************************************************************************
* Main code
*******************************************************************************/

char** get_file_lines(void *filepath){
    FILE *f = fopen(filepath, "r");
    if(f == NULL) {
        perror("Error reading input file!");
        exit(1);
    }

    // Count line numbers for array of lines (we load them in memory right away)
    int character = 0;
    int num_lines = 0;
    while(!feof(f))
    {
        character = fgetc(f);
        if(character == '\n')
            num_lines++;
    }

    printf("%d lines found.\n", num_lines);

    // Malloc output array of strings
    char** input_file_lines = malloc(num_lines * sizeof(char*) * CHUNKSIZE);
    int i = 0;
    for (i =0 ; i < num_lines; ++i)
        input_file_lines[i] = malloc(CHUNKSIZE * sizeof(char));

    // Reading buffer
    char * single_read[CHUNKSIZE];

    // Close and re-open file (EOF)
    rewind(f);
    i = 0;

    // Read file and store in array
    while(fgets(&single_read, CHUNKSIZE, f) != NULL && i <= num_lines) {
        strcpy(input_file_lines[i], single_read);
        i++;
    }
    printf("\n");
    fflush(stdout);
    fclose(f);

    //FREE MEMORY
    return input_file_lines;

}


int main()
{
    const char string[] = "iloveyou";
    char * hash = crypt(string, MD5);
    printf("%s\n",hash);
    /* **************************** */

    char** lines = get_file_lines("training-shadow.txt");
    for (int i = 0; i < 3; i++)
    {
        printf("%s", lines[i]);
        printf("\n");
    }

    return 0;
}
