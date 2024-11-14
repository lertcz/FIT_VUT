    /************************************************/
    /*                                              */		
    /*      1st Project - Working with text         */
    /*                                              */
    /*         Michal Sestak (xsestam00)            */
    /*                                              */
    /*              14. 10. 2024                    */
    /*                                              */
    /*      Introduction to Programming Systems     */
    /*                                              */
    /*              1st year BITP                   */
    /*                                              */
    /*      VUT Faculty of Information Technology   */
    /*                                              */
    /************************************************/  

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_LENGTH 100

bool string_is_number(char *number) {
    int index = 0;
    while (number[index] != '\0' && number[index] != '\r') {
        if (number[index] < '0' || number[index] > '9') { //
            return false;
        }

        index++;
    }
    return true;
}

void to_lowercase(char *string) {
    int index = 0;
    while (string[index] != '\0') {
        if ('A' <= string[index] && string[index] <= 'Z') {
            string[index] += 32;
        }

        index++;
    }
}

/**
 * Removes all non '\n' and '\r' characters then creates a new NULL terminator at the end
 */
void trim_string(char *string) {
    int string_index = 0;
    int trimmed_string_index = 0;

    while (string[string_index] != '\0') {
        if (string[string_index] != '\n' && string[string_index] != '\r') {
            string[trimmed_string_index] = string[string_index];
            trimmed_string_index++;
        }

        string_index++;
    }

    string[trimmed_string_index] = '\0';
}

// DELETE ME
void print_raw(char *input_string) {
    int index = 0;

    printf("RAW: ");
    while (input_string[index] != '\0') {
        switch (input_string[index]) {
            case '\n':
                printf("\\n");
                break;
            case '\t':
                printf("\\t");
                break;
            case '\r':
                printf("\\r");
                break;
            case '\\':
                printf("\\\\");
                break;
            default:
                printf("%c", input_string[index]);
        }
        index++;
    }
    printf("\n");
}

int str_len(char *str) {
    int length = 0;

    while (str[length] != '\0' && str[length] != '\r') {
        length++;
    }

    return length;
}

/** 
 * @return Returns an index of the first occurance of the substring, or -1 if the substring is not found.
 */
int find_substring(char *string, char *substring) {
    int string_len = str_len(string);
    int substring_len = str_len(substring);

    if (substring_len > string_len) {
        return -1;
    }

    for (int search_idx = 0; search_idx < (string_len - (substring_len - 1)); search_idx++) {
        for (int subs_idx = 0; subs_idx < substring_len; subs_idx++) {
            // character missmatch
            if (string[search_idx + subs_idx] != substring[subs_idx]) {
                break;
            }

            // characters match
            if (subs_idx == substring_len-1) {
                return search_idx;
            }
        }
    }

    return -1;
}

int find_substring_discontinuous(char *string, char *substring) {
    int string_len = str_len(string);
    int substring_len = str_len(substring);

    if (substring_len > string_len) {
        return -1;
    }

    for (int search_idx = 0; search_idx < (string_len - (substring_len - 1)); search_idx++) {
        int number_of_matches = 0;

        for (int subs_idx = search_idx; subs_idx < string_len; subs_idx++) {
            if (string[subs_idx] == substring[number_of_matches]) {
                number_of_matches++;
            }

            if (number_of_matches == substring_len) {
                return search_idx;
            }
        }
    }

    return -1;
}

bool is_char_in_string(char *string, char looking_for) {
    for (int index = 0; index < str_len(string); index++) {
        if (string[index] == looking_for) {
            return true;
        }
    }

    return false;
}

/**
 * Altered find_substring to acomodate for the multiple characters per one tnine number
 * 
 * @return Returns an index of the first occurance of the tnine, or -1 if the tnine is not found.
 */
int find_tnine_in_string(char *string, char *tnine) {
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

    if (is_char_in_string(tnine, '1')) {
        return -1;
    }

    int string_len = str_len(string);
    int tnine_length = str_len(tnine);

    if (tnine_length > string_len) {
        return -1;
    }

    for (int search_idx = 0; search_idx < (string_len - (tnine_length - 1)); search_idx++) {
        for (int tnine_idx = 0; tnine_idx < tnine_length; tnine_idx++) {
            char *selected_tnine_code = tnine_conversion_table[tnine[tnine_idx] - 48]; // converts char to a number which is then looked up in the tnine table

            // checks if current character is not contained in the tnine table value
            if (!is_char_in_string(selected_tnine_code, string[search_idx + tnine_idx])) {
                break;
            }

            // tnine match
            if (tnine_idx == tnine_length-1) {
                return search_idx;
            }
        }
    }

    return -1;
}

int find_tnine_in_string_discontinuous(char *string, char *tnine) {
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

    // tnine 1 ?!?!
    if (is_char_in_string(tnine, '1')) {
        return -1;
    }

    int string_len = str_len(string);
    int tnine_length = str_len(tnine);

    if (tnine_length > string_len) {
        return -1;
    }

    for (int search_idx = 0; search_idx < (string_len - (tnine_length - 1)); search_idx++) {
        int number_of_matches = 0;

        for (int tnine_idx = search_idx; tnine_idx < string_len; tnine_idx++) {
            char *selected_tnine_code = tnine_conversion_table[tnine[number_of_matches] - 48]; // converts char to a number which is then looked up in the tnine table

            // checks if current character is not contained in the tnine table value
            if (is_char_in_string(selected_tnine_code, string[search_idx + tnine_idx])) {
                number_of_matches++;
            }

            // tnine match
            if (number_of_matches == tnine_length) {
                return search_idx;
            }
        }
    }

    return -1;
}

struct argument_payload {
    int status;
    char *tnine_number;
};

/**
 * Validates the input parameters for the program.
 * 
 * Status code:
 *  0 - continuous (normal)
 *  1 - discontinuous (-s flag)
 * -1 - Error, incorrect argument
 *
 * @return Returns a payload with status and a number given by the user.
 */
struct argument_payload check_input_parameters(int argc, char *argv[]) {
    // { status_code, number }
    static struct argument_payload output = { -1, "" };

    // check that there is max 3 arguments
    if (argc > 3) {
        fprintf(stderr, "Too many arguments!\n");
        fprintf(stderr, "Usage: ./tnine [-s] NUMBER\n");
        return output;
    }

    // only number was given
    if (argc == 2) {
        if (!string_is_number(argv[1])) {
            fprintf(stderr, "Given argument is not a number!\n");
            fprintf(stderr, "Usage: ./tnine [-s] NUMBER\n");
            return output;
        }

        output.status = 0;
        output.tnine_number = argv[1];
        return output;
    }
    // check if first arg is the correct flag, followed by a number
    else if (argc == 3) {
        if (strcmp(argv[1], "-s") != 0) {
            fprintf(stderr, "First argument is an unexpected flag!\n");
            fprintf(stderr, "Usage: ./tnine [-s] NUMBER\n");
            return output;
        }
        else if (!string_is_number(argv[2])) {
            fprintf(stderr, "Second argument is not a number!\n");
            fprintf(stderr, "Usage: ./tnine [-s] NUMBER\n");
            return output;
        }

        output.status = 1;
        output.tnine_number = argv[2];
        return output;
    }
    // no arguments given. Returns empty string that will be ignored by filter_trough_contact_input
    else {
        printf("else!\n");
        output.status = 0;
        return output;
    }
}

int filter_trough_contact_input(char *tnine, bool continuous_search) {
    char input_name[MAX_LENGTH];
    char input_number[MAX_LENGTH];
    bool found_a_match = false;

    // read lines until the end
    while (!feof(stdin)) {
        if (fgets(input_name, sizeof(input_name), stdin) == NULL) {
            fprintf(stderr, "Problem with loading contact's name.\n");
            return 1;
        }

        if (fgets(input_number, sizeof(input_number), stdin) == NULL) {
            fprintf(stderr, "Problem with loading contact's number.\n");
            return 1;
        }

        // handle the input and make it more suitable for manipulation
        trim_string(input_name);
        trim_string(input_number);
        to_lowercase(input_name);

        // check if tnine was not given to the function
        if (str_len(tnine) == 0) {
            found_a_match = true;
            printf("%s, %s\n", input_name, input_number);
        }
        else if (continuous_search) {
            // check if the given input contains the tnine as a pure number
            if (find_substring(input_number, tnine) >= 0) {
                found_a_match = true;
                printf("%s, %s\n", input_name, input_number);
            }
            // decode tnine numbers as individual characters 
            else if (find_tnine_in_string(input_name, tnine) >= 0) {
                found_a_match = true;
                printf("%s, %s\n", input_name, input_number);
            }
        }
        // alternative version of same algorithms that account for holes in sequence
        else {
            if (find_substring_discontinuous(input_number, tnine) >= 0) {
                found_a_match = true;
                printf("%s, %s\n", input_name, input_number);
            }
            else if (find_tnine_in_string_discontinuous(input_name, tnine) >= 0) {
                found_a_match = true;
                printf("%s, %s\n", input_name, input_number);
            }
        }

    }

    if (!found_a_match) {
        printf("Not found\n");
    }

    return 0;
}

int main(int argc, char *argv[]) {
    struct argument_payload argument_data = check_input_parameters(argc, argv);

    // close the program if there is an error in arguments
    if (argument_data.status == -1) {
        return 1;
    }

    bool continuous_search = argument_data.status == 0; 

    filter_trough_contact_input(argument_data.tnine_number, continuous_search);

    //printf("\n");

    return 0;
}
