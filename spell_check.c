#include <ctype.h>
#include <stdio.h>
#include <string.h>

#include "dictionary.h"

#define MAX_CMD_LEN 128

// A helper function to spell check a specific file
// 'file_name': Name of the file to spell check
// 'dict': A dictionary containing correct words
int spell_check_file(const char *file_name, const dictionary_t *dict) {
    //The case that the specified file does not exist or cannot be opened
    FILE *fh = fopen(file_name, "r");
    if (fh == NULL) {
        return -1;
    }

    char word[MAX_CMD_LEN];
    int ch = 0;
    int i = 0;

    while ((ch = fgetc(fh)) != EOF) {
        if (isspace(ch)) {
            if (i > 0) { //if has get a whole word
                word[i] = '\0'; //end the string
                if (!dict_find(dict, word)) {
                    printf("%s[X] ", word); //don't find the word in dict
                } else {
                    printf("%s ", word); //find the word in dict
                }
                i = 0;
            }
            // meet the new line
            if (ch == '\n') {
                printf("\n");
            }
        } else {
            word[i++] = ch;
        }
    }

    // The last word on the file if no space after it
    if (i > 0) {
        word[i] = '\0';
        if (!dict_find(dict, word)) {
            printf("%s[X]\n", word);
        } else {
            printf("%s\n", word);
        }
    }
    fclose(fh);
    return 0;
}

/*
 * This is in general *very* similar to the list_main file seen in lab
 */
int main(int argc, char **argv) {
    dictionary_t *dict = NULL;
    char cmd[MAX_CMD_LEN];
    dict = create_dictionary();

    //User input 2 arg in the command line
    if (argc == 2) { //second command is dictionary file's name
        strcpy(cmd, argv[1]);
        //read the dictionary file to the dictionary
        dictionary_t *dict2 = NULL;
        dict2 = read_dict_from_text_file(cmd);
        if (dict2 == NULL) {
            printf("Failed to read dictionary from text file\n");
            dict_free(dict);
            return 0; //Exit the program
        } else {
            dict_free(dict); //free the old dictionary
            dict = dict2;
            printf("Dictionary successfully read from text file\n");
        }
    }

    //User input 3 arg in the command line
    if (argc == 3) {
        char cmd2[MAX_CMD_LEN];
        strcpy(cmd, argv[1]); //dictionary file's name
        //read the dictionary file to the dictionary
        dictionary_t *dict2 = NULL;
        dict2 = read_dict_from_text_file(cmd);
        if (dict2 == NULL) {
            //fail to read
            printf("Failed to read dictionary from text file\n");
            dict_free(dict);
            return 0; //Exit the program
        } else {
            dict_free(dict);
            dict = dict2;
            printf("Dictionary successfully read from text file\n");
            strcpy(cmd2, argv[2]); //the file that want spell check
            int success = spell_check_file(cmd2, dict);
            if (success == -1) {
                printf("Spell check failed\n");
                dict_free(dict);
                return 0; //Exit the program
            }
        }
        dict_free(dict);
        return 0;
    }

    printf("CSCI 2021 Spell Check System\n");
    printf("Commands:\n");
    printf("  add <word>:              adds a new word to dictionary\n");
    printf("  lookup <word>:           searches for a word\n");
    printf("  print:                   shows all words currently in the dictionary\n");
    printf("  load <file_name>:        reads in dictionary from a file\n");
    printf("  save <file_name>:        writes dictionary to a file\n");
    printf("  check <file_name>: spell checks the specified file\n");
    printf("  exit:                    exits the program\n");

    while (1) {
        printf("spell_check> ");
        if (scanf("%s", cmd) == EOF) {
            printf("\n");
            break;
        }

        if (strcmp("exit", cmd) == 0) {
            break;
        }

        // TODO Add cases for the other commands
        // Read in the command and any additional arguments (where needed)

        else if (strcmp("add", cmd) == 0) {
            scanf("%s", cmd);
            if (dict_insert(dict, cmd) == -1) {
                printf("Fail to add\n");
            }
        }

        // Lookup word in dictionary
        else if (strcmp("lookup", cmd) == 0) {
            scanf("%s", cmd);
            if (dict_find(dict, cmd)) {
                printf("'%s' present in dictionary\n", cmd);
            } else {
                printf("'%s' not found\n", cmd);
            }
        }

        // Print all words in dictionary
        else if (strcmp("print", cmd) == 0) {
            dict_print(dict);
        }

        // Load dictionary from file
        else if (strcmp("load", cmd) == 0) {
            dictionary_t *dict2 = NULL;
            scanf("%s", cmd); //record the file name
            dict2 = read_dict_from_text_file(cmd);
            if (dict2 == NULL) {
                printf("Failed to read dictionary from text file\n");
            } else {
                dict_free(dict); //free the old dictionary
                dict = dict2;
                printf("Dictionary successfully read from text file\n");
            }
        }

        // Save dictionary to file
        else if (strcmp("save", cmd) == 0) {
            scanf("%s", cmd); //record the file name
            int success = write_dict_to_text_file(dict, cmd);
            if (success == -1) {
                printf("Failed to write dictionary to text file\n");
            }
            if (success == 0) {
                printf("Dictionary successfully written to text file\n");
            }
        }

        // Spell check a file
        else if (strcmp("check", cmd) == 0) {
            scanf("%s", cmd); //record the file name
            int success = spell_check_file(cmd, dict);
            if (success == -1) {
                printf("Spell check failed\n");
            }
        }

        else {
            printf("Unknown command %s\n", cmd);
        }
    }

    dict_free(dict);
    return 0;
}
