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
#include <stdint.h>

/******************************************************************************
* Global definitions
*******************************************************************************/
#define MD5 "$1$QM"
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
    while(fgets((char* restrict) &single_read, CHUNKSIZE, f) != NULL && i <= (num_lines + 1)) {
        strcpy(input_file_lines[i], (const char * restrict) single_read);
        i++;
    }

    // Set array length as first element array
    input_file_lines[0] = (char*) num_lines;

    fclose(f);
    return input_file_lines;

}

int crack_password(char** top250, int len_top250, char* pwd_hash) {
    for (int i = 1; i < 250; i++)
    {
        // Clean string of return character
        top250[i][strcspn(top250[i], "\n")] = 0;

        char* hash = crypt(top250[i], MD5);
        if (!strcmp(hash, pwd_hash))
        {
            return 1;
        }
    }
    return 0;

}

char** combine_words() {
    return NULL;
}

char** split_shadow_file(char** input, int num_lines) {

    // Malloc new array to have user names and hashed passwords
    char** users_and_hashes = malloc((num_lines*2 + 1) * sizeof(char*) * 20);
    int i = 1;
    for (i = 1 ; i < num_lines; ++i)
        users_and_hashes[i] = malloc(20 * sizeof(char));

    char *user_id;
    char *hash;
    int k = 0;

    for (int j = 1; j < num_lines; j++)
    {
        user_id = strtok(input[j], ":");
        hash = strtok(NULL, ":");
        users_and_hashes[k++] = user_id;
        users_and_hashes[k++] = hash;
    }
    return users_and_hashes;

}


int main()
{
    /* **************************** */
    printf("\n_____________________________________________________\n");

    // Read file with hashed passwords
    char** lines = get_file_lines("training-shadow.txt");
    int num_lines = lines[0];
    int count = 0;

    // Split user_id and hashed password into array (each pair of two in array)
    char **users_and_hashes = split_shadow_file(lines, num_lines);
    // Read top250
    char** top250 = get_file_lines("dictionary/clean_top250.txt");

    for (int i = 0; i < num_lines; i++)
    {
        if (i % 2 == 1 && i != 0)
            count += crack_password(top250, num_lines, users_and_hashes[i]);
    }
    printf("Cracked %d out of %d passwords!\n", count, num_lines);

    return 0;
}
