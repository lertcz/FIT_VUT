    /************************************************/
    /*                                              */		
    /*      1st Project - Working with text         */
    /*                                              */
    /*         Michal Sestak (xsestam00)            */
    /*                                              */
    /*              17. 10. 2024                    */
    /*                                              */
    /*      Introduction to Programming Systems     */
    /*                                              */
    /*              1st year BITP                   */
    /*                                              */
    /*      VUT Faculty of Information Technology   */
    /*                                              */
    /************************************************/  

#include <stdio.h>    // standard input and output functions
#include <stdlib.h>   // standard library functions
#include <stdbool.h>  // boolean type support

// define the maximum length for input lines
#define MAX_LENGTH 100

// define return status codes
#define SUCCESS 0 // indicates successful execution
#define ERROR 1   // indicates an error occurred
#define FUNCTION_ERROR -1 // function operation did not succeed

bool string_is_number(char *number) {
    int index = 0;

    // iterate through the string, checking that each character is a digit
    while (number[index] != '\0' && number[index] != '\r') {
        if (number[index] < '0' || number[index] > '9') { //
            return false;
        }

        index++;
    }

    return true; // all characters are digits
}

void to_lowercase(char *string) {
    int index = 0;

    // iterate through the string, converting uppercase letters to lowercase
    while (string[index] != '\0') {
        if ('A' <= string[index] && string[index] <= 'Z') {
            string[index] += 32; // convert uppercase to lowercase
        }

        index++;
    }
}

int string_length(char *string) {
    int length = 0;

    while (string[length] != '\0' && string[length] != '\r') {
        length++;
    }

    return length;
}

bool strings_equal(char *first_string, char *second_string) {
    // only non NULL strings can be compared
    if (first_string == NULL || second_string == NULL) {
        return false;
    }

    // strings have to be equal length
    if (string_length(first_string) != string_length(second_string)) {
        return false;
    }

    // check individual characters
    for (int idx = 0; idx < string_length(second_string); idx++) {
        if (first_string[idx] != second_string[idx]) {
            return false;
        }
    } 

    return true;
}

bool char_in_string(char *string, char looking_for) {
    for (int index = 0; index < string_length(string); index++) {
        if (string[index] == looking_for) {
            return true;
        }
    }

    return false;
}

/** 
 * @param string The reference string in which to search for the substring.
 * @param substring The string to search for within the reference string.
 * @param continuous If true, searches for a continuous (uninterrupted) occurrence of the substring.
 * 
 * @return Returns an index of the first occurance of the substring, or FUNCTION_ERROR if the substring is not found.
 */
int find_substring(char *string, char *substring, bool continuous) {
    int string_len = string_length(string);
    int substring_len = string_length(substring);

    if (substring_len > string_len) {
        return FUNCTION_ERROR;
    }

    // string length optimised for current search type
    int search_length = continuous ? (string_len - (substring_len - 1)) : (string_len);
    int discontinuous_matches = 0;

    for (int search_idx = 0; search_idx < search_length; search_idx++) {
        if (continuous) {
            for (int subs_idx = 0; subs_idx < substring_len; subs_idx++) {
                // character missmatch
                if (string[search_idx + subs_idx] != substring[subs_idx]) {
                    break;
                }

                // characters match
                if (subs_idx == substring_len - 1) {
                    return search_idx;
                }
            }
        }
        // discontinuous search
        else {
            if (string[search_idx] == substring[discontinuous_matches]) {
                discontinuous_matches++;
            }

            if (discontinuous_matches == substring_len) {
                return search_idx;
            }
        }
    }

    return FUNCTION_ERROR;
}

/**
 * Modified version of find_substring to account for multiple characters mapped to a single T9 number.
 * 
 * @param string The reference string in which to search for the T9 representation.
 * @param tnine The T9 string to search for, where each character may correspond to multiple letters.
 * @param continuous If true, searches for a continuous (uninterrupted) match of the T9 representation.
 * 
 * @return Returns an index of the first occurance of the T9 string, or FUNCTION_ERROR if the T9 string is not found.
 */
int find_tnine_in_string(char *string, char *tnine, bool continuous) {
    // exit immediately if there is '1' in the tnine code, as '1' typically does not map to any letters in T9
    if (char_in_string(tnine, '1')) {
        return FUNCTION_ERROR;
    }

    char tnine_conversion_table[10][4] = {
        "+",
        "",
        "abc",  // 2
        "def",  // 3
        "ghi",  // 4
        "jkl",  // 5
        "mno",  // 6
        "pqrs", // 7
        "tuv",  // 8
        "wxyz"  // 9
    };

    int string_len = string_length(string);
    int tnine_length = string_length(tnine);

    // if the T9 string is longer than the reference string, it's impossible to find a match, so return FUNCTION_ERROR
    if (tnine_length > string_len) {
        return FUNCTION_ERROR;
    }

    // determine the length of the search based on the search type (continuous or discontinuous)
    int search_length = continuous ? (string_len - (tnine_length - 1)) : (string_len);
    int discontinuous_matches = 0;

    for (int search_idx = 0; search_idx < search_length; search_idx++) {
        if (continuous) {
            for (int tnine_idx = 0; tnine_idx < tnine_length; tnine_idx++) {
                // check if the current character from the string is not in the T9 character set
                // example: 'b' is in the T9 character set "abc" (mapped to the digit 2).
                if (!char_in_string(tnine_conversion_table[tnine[tnine_idx] - 48], string[search_idx + tnine_idx])) {
                    break;
                }

                // if the entire T9 string matches, return the starting index.
                if (tnine_idx == tnine_length - 1) {
                    return search_idx;
                }
            }
        }
        // discontinuous search (non-continuous match of characters)
        else {
            // check if the current character from the string is in the T9 character set
            if (char_in_string(tnine_conversion_table[tnine[discontinuous_matches] - 48], string[search_idx])) {
                discontinuous_matches++;
            }
            
            // if all characters of the T9 string match, return the current index.
            if (discontinuous_matches == tnine_length) {
                return search_idx;
            }
        }
    }

    return FUNCTION_ERROR;
}

/**
 * Reads a line of input from stdin, stores it in the modified_string, 
 * and ensures the input does not exceed MAX_LENGTH. 
 * Ignores carriage returns ('\r').
 *
 * @param input_line A pointer to the string where the input will be stored.
 * 
 * @return The length of the input string, or FUNCTION_ERROR (-1) if the input exceeds MAX_LENGTH.
 */
int read_input_line(char *input_line) {
    int length = 0;
    char c;

    while ((c = getchar()) != EOF && c != '\n') {
        // check if current length is not greater than MAX_LENGTH and current char is not a return carriage
        if (length + 1 > MAX_LENGTH && c != '\r') {
            fprintf(stderr, "Input line is over limit (%d characters)\n", MAX_LENGTH);
            return ERROR;
        }

        // ignore return carriage
        if (c != '\r') {
            input_line[length++] = c;
        }
    }

    // add NULL terminator at the end
    input_line[length] = '\0';

    // check if program is at the End Of File
    if (c == EOF) {
        return EOF;
    }

    return SUCCESS;
}

/**
 * Reads a contact's name and number from input.
 * 
 * @param name   A pointer to the string where the contact's name will be stored.
 * @param number A pointer to the string where the contact's number will be stored.
 * 
 * @return SUCCESS (0) if both the name and number are read successfully, 
 *         ERROR (1) if there is an issue reading one of them, 
 *         EOF if the end of input is reached.
 */
int read_contact(char *name, char *number) {
    switch (read_input_line(name))
    {
        case ERROR:
            fprintf(stderr, "Problem with loading contact's name.\n");
            return ERROR;

        case EOF:
            return EOF;
    }

    switch (read_input_line(number))
    {
        case ERROR:
            fprintf(stderr, "Problem with loading contact's number.\n");
            return ERROR;

        case EOF:
            return EOF;
    }

    // if no error was encountered, convert name to lowercased right away
    to_lowercase(name);

    return SUCCESS;
}

/**
 * Validates the input parameters for the program.
 * 
 * @param tnine_number set to the user input number if no error occurs
 * @param continuous set to false if the -s flag is present, indicating a discontinuous search.
 * 
 * @return status code: 0 for success, 1 if an error occurred.
 */
int check_input_parameters(int argc, char *argv[], char** tnine_number, bool *continuous) {
    // check that there is no more than 3 arguments
    if (argc > 3) {
        fprintf(stderr, "Too many arguments!\n");
        fprintf(stderr, "Usage: ./tnine [-s] NUMBER\n");
        return ERROR;
    }
    
    // initialize the variables to default values
    *tnine_number = "";
    *continuous = true;

    // only number was given
    if (argc == 2) {
        if (!string_is_number(argv[1])) {
            fprintf(stderr, "Given argument is not a number!\n");
            fprintf(stderr, "Usage: ./tnine [-s] NUMBER\n");
            return ERROR;
        }

        // set the T9 number to the user input
        *tnine_number = argv[1];
        return SUCCESS;
    }
    // if there are 3 arguments, check if the first is the correct flag (-s) followed by a number
    else if (argc == 3) {
        if (!strings_equal(argv[1], "-s")) {
            fprintf(stderr, "First argument is an unexpected flag!\n");
            fprintf(stderr, "Usage: ./tnine [-s] NUMBER\n");
            return ERROR;
        }
        else if (!string_is_number(argv[2])) {
            fprintf(stderr, "Second argument is not a number!\n");
            fprintf(stderr, "Usage: ./tnine [-s] NUMBER\n");
            return ERROR;
        }
        
        // set the search mode to discontinuous (-s flag) and update the T9 number
        *continuous = false;
        *tnine_number = argv[2];
        return SUCCESS;
    }
    
    // if no arguments were given, leave tnine_number and continuous as default values
    return SUCCESS;
}

int filter_trough_contact_input(char *tnine, bool continuous_search) {
    // allocate space for contact input (+1 for null terminator)
    char input_name[MAX_LENGTH + 1], input_number[MAX_LENGTH + 1];
    bool found_a_match = false;
    int read_result;

    // read contacts until the EOF
    do {
        read_result = read_contact(input_name, input_number);

        // check for errors in reading contact
        if (read_result == ERROR) {
            return ERROR;
        }

        // if no T9 search pattern was provided, display the contact
        if (string_length(tnine) == 0) {
            found_a_match = true;
            printf("%s, %s\n", input_name, input_number);
        }

        // check if the contact's number contains the T9 pattern as a pure number
        if (find_substring(input_number, tnine, continuous_search) >= 0) {
            found_a_match = true;
            printf("%s, %s\n", input_name, input_number);
        }

        // if the T9 input starts with '0', replace it with '+'
        // this is useful for cases like searching for international numbers like +420
        else if (tnine[0] == '0') {
            // change the first '0' to '+' for international number search (+420)
            tnine[0] = '+';

            // check the input_number with the modified T9 pattern
            if (find_substring(input_number, tnine, continuous_search) >= 0) {
                found_a_match = true;
                printf("%s, %s\n", input_name, input_number);
            }

            // revert the change to preserve the original T9 input for subsequent searches
            tnine[0] = '0';
        }

        // check if the contact's name contains a T9 match (using T9 character mapping)
        if (find_tnine_in_string(input_name, tnine, continuous_search) >= 0) {
            found_a_match = true;
            printf("%s, %s\n", input_name, input_number);
        }
    } while(read_result != EOF);

    // no matches were found
    if (!found_a_match) {
        printf("Not found\n");
    }

    return SUCCESS;
}

int main(int argc, char *argv[]) {
    char *tnine_number;
    bool continuous_search;

    // Validate input parameters and exit if there's an error.
    if (check_input_parameters(argc, argv, &tnine_number, &continuous_search) == 1) {
        return ERROR;
    }

    // Filter contacts based on the provided T9 number and search mode.
    filter_trough_contact_input(tnine_number, continuous_search);

    return SUCCESS;
}
