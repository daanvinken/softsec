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
#define CHUNKSIZE 50

/******************************************************************************
* Main code
*******************************************************************************/

void read_file(void *filepath){
    FILE *f = fopen("training-shadow.txt", "r");
    if(f == NULL) {
        perror("Error reading input file!");
        exit(1);
    }

    char single_read[CHUNKSIZE];

    while(fgets(single_read, sizeof(single_read), f) != NULL) {
        printf("%s", single_read);
    }
    fflush(stdout);
    fclose(f);
}

// int main(int argc, char const *argv[])
int main()
{
    const char string[] = "iloveyou";
    char * hash = crypt(string, MD5);
    printf("%s\n",hash);

    read_file("testing-shadow.txt");

    return 0;
}
