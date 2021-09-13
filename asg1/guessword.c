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
#define HASH_CHUNKSIZE 20

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
    input_file_lines[0] = num_lines;

    fclose(f);
    return input_file_lines;

}

int crack_password(char** hashed_guesses, char* pwd_hash) {
    for (int i = 1; i < hashed_guesses[0]; i++)
    {
        if (!strcmp(hashed_guesses[i], pwd_hash))
        {
            return i;
        }
    }
    return 0;

}

char** combine_word(char** guesses, int len_guesses, int first_word_idx) {
    // Allocate big array of combined words
    char** combined_words = malloc((len_guesses + 1) * sizeof(char*) * HASH_CHUNKSIZE);

    char* first_word = guesses[first_word_idx];

    // Concatenate all strings
    for (int i = 1; i < len_guesses; i++)
    {
            // Should be done at earlier stage
            guesses[i][strcspn(guesses[i], "\n")] = 0;
            char* tmp_char = malloc(sizeof(char) * CHUNKSIZE);
            combined_words[i] = malloc(HASH_CHUNKSIZE * sizeof(char));
            strncpy(tmp_char, first_word, HASH_CHUNKSIZE);
            strcat(tmp_char, guesses[i]);
            combined_words[i] = malloc(HASH_CHUNKSIZE * sizeof(char));
            strncpy(combined_words[i], tmp_char, HASH_CHUNKSIZE);
            free(tmp_char);
    }

    return combined_words;
}

char** split_shadow_file(char** input, int num_lines) {

    // Malloc new array to have user names and hashed passwords
    char** users_and_hashes = malloc((num_lines*2 + 1) * sizeof(char*) * HASH_CHUNKSIZE);
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

char** hash_guesses(char** guesses, int len_guesses, char** salt) {
    char** hashed_guesses = malloc((len_guesses+1) * sizeof(char*) * HASH_CHUNKSIZE);
    char* tmp_hash;
    for (int i = 1 ; i < (len_guesses + 1); ++i)
        hashed_guesses[i] = malloc(HASH_CHUNKSIZE * sizeof(char));

    for (int i = 1; i < len_guesses; i++)
    {
        // Clean string of return character
        guesses[i][strcspn(guesses[i], "\n")] = 0;
        tmp_hash = crypt(guesses[i], MD5);
        strcpy(hashed_guesses[i], (const char * restrict) tmp_hash);
    }
    hashed_guesses[0] = len_guesses;
    return hashed_guesses;
}


int main()
{
    printf("\n_____________________________________________________\n\n");

    // Read file with hashed passwords
    char** lines = get_file_lines("training-shadow.txt");
    int num_shadows = 1000;
    int count = 0;
    int old_count = 0;

    // Split user_id and hashed password into array (each pair of two in array)
    char **users_and_hashes = split_shadow_file(lines, num_shadows);

    // Read preprocessed input file (possible passwords)
    char** guesses = get_file_lines("dictionary/preprocessed_lower.txt");

    int found_index;

    for (int multiplier = 0; multiplier < 10; multiplier++)
    {

        // Loop for guesses (j)
        for (int j = (multiplier * 1000 + 1); j < ((multiplier + 1) * 1000); j++)
        {
            char** combined_guesses = combine_word(guesses, guesses[0], j);
            char** hashed_guesses = hash_guesses(combined_guesses, guesses[0], MD5);
            for (int i = 1; i < num_shadows; i+=2)
            {
                found_index = crack_password(hashed_guesses, users_and_hashes[i]);
                if (found_index){
                    printf("%s:%s\n", users_and_hashes[i-1], combined_guesses[found_index]);
                }
            }
        }
        printf("multiplier = %d\n", multiplier);
    }



    return 0;
}
