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

    // Malloc output array of strings
    char** input_file_lines = malloc((num_lines + 1) * sizeof(char*) * CHUNKSIZE);
    int i = 1;
    for (i = 1 ; i < (num_lines + 1); ++i)
        input_file_lines[i] = malloc(CHUNKSIZE * sizeof(char));

    // Reading buffer
    char * single_read[CHUNKSIZE];

    // Reset file pointer to beginning
    rewind(f);
    i = 1;

    // Read file and store in array
    while(fgets(&single_read, CHUNKSIZE, f) != NULL && i <= (num_lines + 1)) {
        strcpy(input_file_lines[i], single_read);
        i++;
    }

    // Set array length as first element array
    input_file_lines[0] = num_lines;

    fclose(f);

    //FREE MEMORY
    return input_file_lines;

}

// char* hash, char* username
char* crack_password() {
    return NULL;
}

char** combine_words() {
    return NULL;
}

char** split_shadow_file(char** input) {
    int num_lines = input[0];
    // Malloc new array to have user names and hashed passwords
    char** users_and_hashes = malloc((num_lines*2 + 1) * sizeof(char*) * 20);
    int i = 1;
    for (i = 1 ; i < num_lines; ++i)
        users_and_hashes[i] = malloc(20 * sizeof(char));

    char *user_id;
    char *hash;
    char *tmp;
    int k = 0;

    for (int j = 1; j < num_lines; j++)
    {
        user_id = strtok(input[j], ":");
        // printf("%s\n", user_id);
        strtok(NULL, "$");
        strtok(NULL, "$");
        tmp = strtok(NULL, "$");
        hash = strtok(tmp, ":");
        // printf("%s\n", hash);
        users_and_hashes[k++] = user_id;
        users_and_hashes[k++] = hash;

    }
    return users_and_hashes;

}


int main()
{
    // const char string[] = "iloveyou";
    // char * hash = crypt(string, MD5);
    // printf("%s\n",hash);
    /* **************************** */
    printf("\n\n__________________________________________\n");

    // Read file with hashed passwords
    char** lines = get_file_lines("training-shadow.txt");
    printf("Number of retrieved hashes: %d\n", lines[0]);
    for (int i = 1; i < 10; i++)
    {
        printf("%s", lines[i]);
        printf("\n");
    }

    char **users_and_hashes = split_shadow_file(lines);
    for (int i = 0; i < 10; i++)
    {
        printf("%s ", users_and_hashes[i]);
        if (i % 2 == 1 && i != 0)
            printf("\n");
    }

    



    return 0;
}
