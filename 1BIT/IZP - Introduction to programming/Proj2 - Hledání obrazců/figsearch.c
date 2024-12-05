    /************************************************/
    /*                                              */		
    /*      2nd Project - Bitmap search             */
    /*                                              */
    /*         Michal Sestak (xsestam00)            */
    /*                                              */
    /*              27. 11. 2024                    */
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
#include <string.h>   // string operations like strcmp
#include <ctype.h>

// define return status codes
#define SUCCESS 0 // indicates successful execution
#define ERROR 1   // indicates an error occurred
#define FUNCTION_ERROR -1 // function operation did not succeed

// define a struct to hold all the Bitmap data
typedef struct {
    int rows;
    int columns;
    int **data;
} Bitmap;

typedef struct {
    int row_start;
    int col_start;
    int row_end;
    int col_end;
} Coordinates;

// function prototypes 
void free_bitmap(Bitmap *bitmap);

// --------------------------------------------------------------------------------
// Main search functions

/**
 * Calculates the length of a vertical line in a bitmap.
 * The function iterates downwards from the given starting row and column, propagating
 * the line length until a stopping condition is met.
 *
 * @param start_row The row index to start from.
 * @param start_col The column index to start from.
 * @param limit_search The maximum length to search for the line. The propagation
 *                     will stop once this limit is reached.
 * @param bitmap A pointer to the `Bitmap` structure.
 *
 * @return The length of the vertical line found, or `FUNCTION_ERROR` if invalid
 *         parameters are provided.
 *
 * Stopping conditions for line propagation:
 * - The line reaches the maximum search limit (`limit_search`).
 * - The next row in the same column contains a value of 0.
 * - The end of the bitmap's rows is reached.
 */
int vline_propagate(int start_row, int start_col, int limit_search, Bitmap *bitmap) {
    // iterate till the end of the bitmap
    for (int line_length = 0; (start_row + line_length) < bitmap->rows; line_length++) {
        // stop if limit was reached
        if (line_length == limit_search)
            return line_length;
        
        // stop if index arrives at the end of the bitmap
        if (start_row + line_length == bitmap->rows-1)
            return line_length + 1;

        // stop if next value is 0
        if (bitmap->data[start_row + line_length + 1][start_col] == 0) 
            return line_length + 1;
    }

    // should only happen if the parameters are wrong
    return FUNCTION_ERROR;
}

/**
 * Calculates the length of a horizontal line in a bitmap.
 * The function iterates horizontally from the given starting row and column, propagating
 * the line length until a stopping condition is met.
 *
 * @param start_row The row index to start from.
 * @param start_col The column index to start from.
 * @param limit_search The maximum length to search for the line. The propagation
 *                     will stop once this limit is reached.
 * @param bitmap A pointer to `Bitmap` structure.
 *
 * @return The length of the horizontal line found, or `FUNCTION_ERROR` if invalid
 *         parameters are provided.
 *
 * Stopping conditions for line propagation:
 * - The line reaches the maximum search limit (`limit_search`).
 * - The next column in the same row contains a value of 0.
 * - The end of the bitmap's columns is reached.
 */
int hline_propagate(int start_row, int start_col, int limit_search, Bitmap *bitmap) {
    // iterate till the end of the bitmap
    for (int line_length = 0; (start_col + line_length) < bitmap->columns; line_length++) {
        // stop if limit was reached
        if (line_length == limit_search)
            return line_length;
        
        // stop if index arrives at the end of the bitmap
        if (start_col + line_length == bitmap->columns-1)
            return line_length + 1;

        // stop if next value is 0
        if (bitmap->data[start_row][start_col + line_length + 1] == 0) {
            return line_length + 1;
        }
    }

    // should only happen if the parameters are wrong
    return FUNCTION_ERROR;
}

/**
 * Calculates the size of a square, starting from the specified position,
 * without exceeding the specified maximum size.
 *
 * The function verifies the presence of all possible squares on a line by checking the horizontal lines 
 * (top and bottom) and the vertical line (right side) of the square. The square is
 * valid if all these lines match the required size.
 *
 * @param start_row The row index to start from.
 * @param start_col The column index to start from.
 * @param max_size The maximum size of the square to check. This value will be adjusted
 *                 if the specified size would exceed the bitmap's boundaries.
 * @param bitmap A pointer to the `Bitmap` structure.
 *
 * @return The size of the largest square that satisfies the conditions, or `FUNCTION_ERROR`
 *         if invalid parameters are provided.
 */
int check_squares_on_line(int start_row, int start_col, int max_size, Bitmap *bitmap) {
    // verify that the max size is not larger than the amount of columns left from the start
    if (start_col + max_size >= bitmap->columns)
        // new coordinates - difference (out of bounds space)
        max_size -= (start_col + max_size) - (bitmap->columns);


    for (int current_size = max_size; current_size > 0; current_size--) {
        int hline_upper = hline_propagate(start_row, start_col, current_size, bitmap);

        if (hline_upper != current_size)
            continue;

        int hline_lower = hline_propagate(start_row + current_size - 1, start_col, current_size, bitmap);
        if (hline_lower != current_size)
            continue;

        int vline_right = vline_propagate(start_row, start_col + current_size - 1, current_size, bitmap);
        if (vline_right != current_size)
            continue;

        return current_size;
    }

    return FUNCTION_ERROR;
}

/**
 * Finds the longest vertical line of consecutive 1s in a bitmap and stores the coordinates
 * of its start and end points in the result pointer.
 *
 * @param bitmap A pointer to the `Bitmap` structure.
 * @param result A pointer to the `Coordinates` structure. The function updates the values
 *               with the start and end coordinates of the longest vertical line found.
 *               If no vertical line is found, the structure is not modified.
 */
void find_vline(Bitmap *bitmap, Coordinates *result) {
    int longest_length = -1; // -1 so a line with a length of 1 (0) is accounted for

    for (int row = 0; row < bitmap->rows; row++) {
        for (int col = 0; col < bitmap->columns; col++) {
            // check if there is no way of finding larger lines anymore
            if (longest_length >= bitmap->rows - row) { // !!! validate if this is correct
                return;
            }

            // only perform calculation when there is 1 on the current index
            if (bitmap->data[row][col] == 1) {
                int line_size = vline_propagate(row, col, -1, bitmap) - 1; // subtract one to transform it into "index form"

                if (line_size > longest_length) {
                    result->row_start = row;
                    result->col_start = col;
                    result->row_end = row+line_size;
                    result->col_end = col;

                    longest_length = line_size;
                }
            }
        }
    }
}

/**
 * Finds the longest horizontal line of consecutive 1s in a bitmap and stores the coordinates
 * of its start and end points in the result pointer.
 *
 * @param bitmap A pointer to the `Bitmap` structure.
 * @param result A pointer to the `Coordinates` structure. The function updates the values
 *               with the start and end coordinates of the longest vertical line found.
 *               If no vertical line is found, the structure is not modified.
 */
void find_hline(Bitmap *bitmap, Coordinates *result) {
    int longest_length = -1; // -1 so a line with a length of 1 (0) is accounted for

    // reverse the order of the loops so if there is no way of finding longer lines, so it can be terminated
    for (int row = 0; row < bitmap->rows; row++) {
        for (int col = 0; col < bitmap->columns; col++) {
            // check if there is no way of finding larger lines anymore
            if (longest_length >= bitmap->columns - col) {
                break;
            }
            
            // only perform calculation when there is 1 on the current index
            if (bitmap->data[row][col] == 1) {
                int line_size = hline_propagate(row, col, -1, bitmap) - 1; // subtract one to transform it into "index form"

                if (line_size > longest_length) {
                    result->row_start = row;
                    result->col_start = col;
                    result->row_end = row;
                    result->col_end = col+line_size;
                    
                    longest_length = line_size;
                }
            }
        }
    }
}

/**
 * Finds the largest square of consecutive 1s in a bitmap and stores the coordinates of
 * the top-left and bottom-right corners in the result pointer.
 *
 * @param bitmap A pointer to the `Bitmap` structure.
 * @param result A pointer to the `Coordinates` structure. The function updates the values
 *               with the start and end coordinates of the longest vertical line found.
 *               If no vertical line is found, the structure is not modified.
 */
void find_square(Bitmap *bitmap, Coordinates *result) {
    int biggest_square = -1;

    for (int row = 0; row < bitmap->rows; row++) {
        for (int col = 0; col < bitmap->columns; col++) {
            // check if there is no way of finding larger squares anymore
            if (biggest_square >= bitmap->rows - row) {
                return;
            }
            
            // only perform calculation when there is 1 on the current index
            if (bitmap->data[row][col] == 1) {
                int vline_left = vline_propagate(row, col, -1, bitmap);
                int square_size = check_squares_on_line(row, col, vline_left, bitmap) - 1; // subtracting 1 so it is in an "index form"

                if (square_size > biggest_square) {
                    result->row_start = row;
                    result->col_start = col;
                    result->row_end = row + square_size;
                    result->col_end = col + square_size;

                    biggest_square = square_size;
                }
            }
        }
    }
}

/**
 * Starts a search mode to find a specific pattern in the bitmap based on the given mode.
 * 
 * @param mode A string specifying the search mode. It can be one of the following:
 *             - "vline" to search for vertical lines,
 *             - "hline" to search for horizontal lines,
 *             - "square" to search for the largest square of 1s.
 * @param bitmap A pointer to the `Bitmap` structure.
 * 
 * Notes:
 * The result of the search is printed in the format: row_start col_start row_end col_end
 */
void start_search_mode(char *mode, Bitmap *bitmap) {
    static Coordinates result = { .row_start=-1, .col_start=-1, .row_end=-1, .col_end=-1 };

    if (strcmp(mode, "vline") == 0) {
        find_vline(bitmap, &result);

    }
    else if (strcmp(mode, "hline") == 0) {
        find_hline(bitmap, &result);
    }
    else if (strcmp(mode, "square") == 0) {
        find_square(bitmap, &result);
    }
    else {
        fprintf(stderr, "Error: Wrong mode supplied to 'start_search_mode'");
        return;
    }

    if (result.row_start == -1 || result.col_start == -1) {
        printf("Not found\n");
    }
    else {
        printf("%d ", result.row_start);
        printf("%d ", result.col_start);
        printf("%d ", result.row_end);
        printf("%d\n", result.col_end);
    }
}

// --------------------------------------------------------------------------------
// Support functions

// help print
// bitmap loading
// parameter check

void print_help() {
    printf("USAGE:\n");
    printf("\t./program [--help | vline | hline | square] FILE\n");
    printf("OPTIONS:\n");
    printf("\t--help     - Show the help message and exit.\n");
    printf("\ttest       - Verify the file's integrity.\n");
    printf("\tvline      - Perform a vertical line search.\n");
    printf("\thline      - Perform a horizontal line search.\n");
    printf("\tsquare     - Perform a square search.\n");
    printf("FILE:\n");
    printf("\tThe bitmap file to be processed (required for all modes except --help).\n");
}

bool verify_bitmap_size(FILE *file, Bitmap *bitmap) {
    // Read rows and column size
    if (fscanf(file, "%d %d", &bitmap->rows, &bitmap->columns) != 2 || // check if scanf loaded both values
        bitmap->rows <= 0 || bitmap->columns <= 0) { // check if both values are positive
        //fprintf(stderr, "Error: Invalid bitmap header (rows and columns).\n");
        fprintf(stderr, "Invalid\n");
        return false;
    }

    return true;
}

bool allocate_bitmap_data(Bitmap *bitmap) {
    // Allocate memory for bitmap rows
    bitmap->data = malloc(bitmap->rows * sizeof(int *));
    if (bitmap->data == NULL) {
        //fprintf(stderr, "Error allocating memory for bitmap rows.\n");
        fprintf(stderr, "Invalid\n");
        return false;
    }

    // Allocate memory for each row's columns
    for (int row = 0; row < bitmap->rows; row++) {
        bitmap->data[row] = malloc(bitmap->columns * sizeof(int));
        if (bitmap->data[row] == NULL) {
            //fprintf(stderr, "Error allocating memory for bitmap columns (row %d).\n", row);
            fprintf(stderr, "Invalid\n");
            // free allocated memory before returning false
            free_bitmap(bitmap);
            return false;
        }
    }
    return true;
}

// verify if data contains onlly 1 and 0
bool validate_bitmap_data(FILE *file, Bitmap *bitmap) {
    for (int row = 0; row < bitmap->rows; row++) {
        for (int col = 0; col < bitmap->columns; col++) {
            // attempt to read an integer
            if (fscanf(file, "%d", &bitmap->data[row][col]) != 1) {
                // look for a char if scanf fails to load int
                // print what caused the issue
                char c;
                if (fscanf(file, "%c", &c) == 1) {
                    // fprintf(stderr, "Error: Invalid data '%c' at row %d, column %d. Expected 0 or 1.\n", c, row, col);
                    fprintf(stderr, "Invalid\n");
                } else {
                    // fprintf(stderr, "Error: Unexpected end of file or input at row %d, column %d.\n", row, col);
                    fprintf(stderr, "Invalid\n");
                }
                
                // invalid data is found
                return false;
            }

            // Validate if the value is either 0 or 1
            if (bitmap->data[row][col] != 0 && bitmap->data[row][col] != 1) {
                // printf("Error: Invalid value '%d' at row %d, column %d. Expected 0 or 1.\n", bitmap->data[row][col], row, col);
                fprintf(stderr, "Invalid\n");
                return false;
            }
        }
    }
    return true;
}

// bool check_unwanted_input(FILE *file) {
//     int ch;
//     while ((ch = fgetc(file)) != EOF) {
//         // ignore white space characters
//         if (!isspace(ch)) {
//             // printf("Error: Unexpected character '%c' encountered in the file.\n", ch);
//             fprintf(stderr, "Invalid\n");
//             return false;
//         }
//     }
//     return true;
// }


/**
 * Loads a bitmap from a file and returns a pointer to a `Bitmap` structure.
 * 
 * @param filename The name of the file to load the bitmap from.
 * 
 * @return A pointer to the `Bitmap` structure if the file is successfully loaded and verified, or NULL if an error occurs.
 */
Bitmap *load_bitmap(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        //fprintf(stderr, "Error while opening bitmap file '%s'\n", filename);
        fprintf(stderr, "Invalid\n");
        return NULL;
    }

    // allocate Bitmap structure
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    if (bitmap == NULL) {
        //fprintf(stderr, "Error while allocating memory for bitmap\n");
        fprintf(stderr, "Invalid\n");
        return NULL;
    }

    if (!verify_bitmap_size(file, bitmap)) {
        fclose(file);
        free_bitmap(bitmap);
        return NULL;
    }

    if (!allocate_bitmap_data(bitmap)) {
        fclose(file);
        return NULL;
    }

    if (!validate_bitmap_data(file, bitmap)) {
        fclose(file);
        free_bitmap(bitmap);
        return NULL;
    }

    // if (!check_unwanted_input(file)) {
    //     fclose(file);
    //     free(bitmap);
    //     return NULL;
    // }

    fclose(file);
    return bitmap;
}

/**
 * Frees the memory allocated for the bitmap structure and its data.
 * 
 * @param bitmap A pointer to the `Bitmap` structure to be freed.
 *               If the pointer is NULL, no action is taken.
 */
void free_bitmap(Bitmap *bitmap) {
    if (bitmap != NULL) {
        if (bitmap->data != NULL) {
            for (int i = 0; i < bitmap->rows; i++) {
                free(bitmap->data[i]);
            }
            free(bitmap->data);
        }
        free(bitmap);
    }
}

/**
 * Validates the input parameters for the program.
 * 
 * @return status code: 
 *         - 0 for success
 *         - 1 if an error occurred
 */
int check_input_parameters(int argc, char *argv[], char **mode, Bitmap **bitmap) {
    // check if the first argument is "--help"
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        print_help();
        return SUCCESS;
    }

    // for other modes, ensure both mode and file are provided
    if (argc != 3) {
        fprintf(stderr, "Incorrect arguments.\n");
        print_help();
        return ERROR;
    }
    
    // check and save the given mode into a variable
    if (
        strcmp(argv[1], "test") == 0 ||
        strcmp(argv[1], "vline") == 0 || 
        strcmp(argv[1], "hline") == 0 || 
        strcmp(argv[1], "square") == 0
        ) 
    {
        *mode = argv[1];
    }
    // handle exceptions
    else {
        fprintf(stderr, "Unknown mode '%s'.\n\n", argv[1]);
        print_help();
        return ERROR;
    }

    // check if verification failed
    Bitmap *bitmap_pointer = load_bitmap(argv[2]);
    if (bitmap_pointer == NULL) {
        //fprintf(stderr, "Invalid\n");
        return ERROR;
    }

    // 
    if (strcmp(*mode, "test") == 0) {
        //print_bitmap(bitmap_pointer);
        //printf("Bitmap in file '%s' loaded without errors\n", argv[2]);
        printf("Valid\n");
        free_bitmap(bitmap_pointer);
        return SUCCESS;
    }

    *bitmap = bitmap_pointer;

    return SUCCESS;
}

int main(int argc, char *argv[]) {
    char *mode = NULL;
    Bitmap *bitmap = NULL;

    // Validate input parameters and exit if there's an error or no mode was selected.
    if (check_input_parameters(argc, argv, &mode, &bitmap) == 1 || mode == NULL) {
        return ERROR;
    }

    // search only if bitmap was returned from the parameters
    if (bitmap != NULL) {
        start_search_mode(mode, bitmap);
        free_bitmap(bitmap);
    }
    
    return SUCCESS;
}
