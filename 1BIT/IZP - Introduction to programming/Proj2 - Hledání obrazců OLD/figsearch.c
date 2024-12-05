    /************************************************/
    /*                                              */		
    /*      2nd Project - Bitmap search             */
    /*                                              */
    /*         Michal Sestak (xsestam00)            */
    /*                                              */
    /*              18. 10. 2024                    */
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

// Coordinates vline_scout() {
//     return;
// }

void find_vline(Bitmap *bitmap, Coordinates *result) {
    for (int row = 0; row < bitmap->rows; row++) {
        for (int col = 0; col < bitmap->columns; col++) {
            if (false) {

            }
        }
    }

    result->col_start = -1;
}

void find_hline(Bitmap *bitmap, Coordinates *result) {
    for (int row = 0; row < bitmap->rows; row++) {
        for (int col = 0; col < bitmap->columns; col++) {

        }
    }

    result->col_start = -1;
}

void find_square(Bitmap *bitmap, Coordinates *result) {
    for (int row = 0; row < bitmap->rows; row++) {
        for (int col = 0; col < bitmap->columns; col++) {

        }
    }
    
    result->col_start = -1;
}

void start_search_mode(char *mode, Bitmap *bitmap) {
    Coordinates result = { .row_start=-1, .col_start=-1, .row_end=-1, .col_end=-1 };

    if (strcmp(mode, "vline")) {
        find_vline(bitmap, &result);
    }
    else if (strcmp(mode, "hline")) {
        find_hline(bitmap, &result);
    }
    else if (strcmp(mode, "square")) {
        find_square(bitmap, &result);
    }
    else {
        fprintf(stderr, "Error: Wrong mode supplied to 'start_search_mode'");
        return;
    }

    if (result.row_start == -1) {
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
    // Read rows and columns
    if (fscanf(file, "%d %d", &bitmap->rows, &bitmap->columns) != 2 || // check if scanf loaded both values
        bitmap->rows <= 0 || bitmap->columns <= 0) { // check if both values are positive
        fprintf(stderr, "Error: Invalid bitmap header (rows and columns).\n");
        return false;
    }

    return true;
}

bool allocate_bitmap_data(Bitmap *bitmap) {
    // Allocate memory for bitmap rows
    bitmap->data = malloc(bitmap->rows * sizeof(int *));
    if (bitmap->data == NULL) {
        fprintf(stderr, "Error allocating memory for bitmap rows.\n");
        return false;
    }

    // Allocate memory for each row's columns
    for (int row = 0; row < bitmap->rows; row++) {
        bitmap->data[row] = malloc(bitmap->columns * sizeof(int));
        if (bitmap->data[row] == NULL) {
            fprintf(stderr, "Error allocating memory for bitmap columns (row %d).\n", row);
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
                char c;
                // print what caused the issue
                if (fscanf(file, "%c", &c) == 1) {
                    fprintf(stderr, "Error: Invalid data '%c' at row %d, column %d. Expected 0 or 1.\n", c, row, col);
                } else {
                    fprintf(stderr, "Error: Unexpected end of file or input at row %d, column %d.\n", row, col);
                }
                
                // invalid data is found
                return false;
            }

            // Validate if the value is either 0 or 1
            if (bitmap->data[row][col] != 0 && bitmap->data[row][col] != 1) {
                printf("Error: Invalid value '%d' at row %d, column %d. Expected 0 or 1.\n", bitmap->data[row][col], row, col);
                return false;
            }
        }
    }
    return true;
}

bool check_unwanted_input(FILE *file) {
    int ch;
    while ((ch = fgetc(file)) != EOF) {
        // ignore white space characters
        if (!isspace(ch)) {
            printf("Error: Unexpected character '%c' encountered in the file.\n", ch);
            return false;
        }
    }

    return true;
}

Bitmap *load_bitmap(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening bitmap file '%s'\n", filename);
        return NULL;
    }

    // allocate Bitmap structure
    Bitmap *bitmap = malloc(sizeof(Bitmap));
    if (bitmap == NULL) {
        fprintf(stderr, "Error allocating memory for bitmap\n");
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

    if (!check_unwanted_input(file)) {
        fclose(file);
        free(bitmap);
        return NULL;
    }

    fclose(file);
    return bitmap;
}

// free the allocated memory
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
 * @return status code: 0 for success, 1 if an error occurred.
 */
int check_input_parameters(int argc, char *argv[], char **mode, Bitmap **bitmap) {
    // check if the first argument is "--help"
    if (argc == 2 && strcmp(argv[1], "--help") == 0) {
        print_help();
        return SUCCESS;
    }

    // for other modes, ensure both mode and file are provided
    if (argc != 3) {
        printf("Error: Incorrect arguments.\n");
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
        fprintf(stderr, "Error: Unknown mode '%s'.\n\n", argv[1]);
        print_help();
        return ERROR;
    }

    // check if verification failed
    Bitmap *bitmap_pointer = load_bitmap(argv[2]);


    if (bitmap_pointer == NULL) {
        fprintf(stderr, "Failed to load bitmap from file: %s\n", argv[2]);
        return ERROR;
    }

    if (strcmp(*mode, "test") == 0) {
        printf("Bitmap in file '%s' loaded without errors\n", argv[2]);
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
