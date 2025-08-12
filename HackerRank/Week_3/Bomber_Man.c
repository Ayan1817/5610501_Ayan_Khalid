#include <assert.h>
#include <ctype.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);

int parse_int(char*);

// Helper function to create a grid and fill it with a character
char** create_grid(int r, int c, char fill_char) {
    char** new_grid = malloc(r * sizeof(char*));
    for (int i = 0; i < r; i++) {
        new_grid[i] = malloc((c + 1) * sizeof(char));
        memset(new_grid[i], fill_char, c);
        new_grid[i][c] = '\0';
    }
    return new_grid;
}

// Helper function to copy a grid
char** copy_grid(int r, int c, char** source_grid) {
    char** new_grid = malloc(r * sizeof(char*));
    for (int i = 0; i < r; i++) {
        new_grid[i] = malloc((c + 1) * sizeof(char));
        strcpy(new_grid[i], source_grid[i]);
    }
    return new_grid;
}

// Helper function to simulate detonation.
// It creates a new grid representing the state after bombs from bomb_locations_grid explode.
char** detonate(int r, int c, char** bomb_locations_grid) {
    // Start with a grid full of bombs, which is the state before detonation
    char** result_grid = create_grid(r, c, 'O');
    
    // Iterate through the reference grid to find where bombs were
    for (int i = 0; i < r; i++) {
        for (int j = 0; j < c; j++) {
            // If a bomb existed at this location...
            if (bomb_locations_grid[i][j] == 'O') {
                // ...clear this cell and its neighbors in the result grid.
                result_grid[i][j] = '.';
                if (i > 0) result_grid[i - 1][j] = '.'; // Up
                if (i < r - 1) result_grid[i + 1][j] = '.'; // Down
                if (j > 0) result_grid[i][j - 1] = '.'; // Left
                if (j < c - 1) result_grid[i][j + 1] = '.'; // Right
            }
        }
    }
    return result_grid;
}

/*
 * Complete the 'bomberMan' function below.
 *
 * The function is expected to return a STRING_ARRAY.
 * The function accepts following parameters:
 * 1. INTEGER n
 * 2. STRING_ARRAY grid
 */
char** bomberMan(int n, int grid_count, char** grid, int* result_count) {
    int r = grid_count;
    int c = strlen(grid[0]);
    *result_count = r;
    
    // Case 1: n = 1. Bomberman does nothing. Grid is unchanged.
    if (n == 1) {
        return copy_grid(r, c, grid);
    }
    
    // Case 2: n is even. The grid becomes completely filled with bombs.
    if (n % 2 == 0) {
        return create_grid(r, c, 'O');
    }
    
    // Case 3: n is odd and n >= 3. The state follows a cycle.
    
    // Calculate the state after the first detonation (state G3).
    // The bombs from the initial grid explode.
    char** grid_after_first_detonation = detonate(r, c, grid);
    
    // This state occurs at n = 3, 7, 11, ..., which is when (n % 4 == 3).
    if (n % 4 == 3) {
        return grid_after_first_detonation;
    }
    
    // The state for n = 5, 9, 13, ..., which is when (n % 4 == 1).
    // This requires a second detonation based on the state G3.
    char** grid_after_second_detonation = detonate(r, c, grid_after_first_detonation);
    
    // Free the memory used by the intermediate grid state.
    for (int i = 0; i < r; i++) {
        free(grid_after_first_detonation[i]);
    }
    free(grid_after_first_detonation);
    
    return grid_after_second_detonation;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int r = parse_int(*(first_multiple_input + 0));

    int c = parse_int(*(first_multiple_input + 1));

    int n = parse_int(*(first_multiple_input + 2));

    char** grid = malloc(r * sizeof(char*));

    for (int i = 0; i < r; i++) {
        char* grid_item = readline();

        *(grid + i) = grid_item;
    }

    int result_count;
    char** result = bomberMan(n, r, grid, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%s", *(result + i));

        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!data) {
            data = '\0';

            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    while (*str != '\0' && isspace(*str)) {
        str++;
    }

    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return '\0';
    }

    if (!*str) {
        return str;
    }

    char* end = str + strlen(str) - 1;

    while (end >= str && isspace(*end)) {
        end--;
    }

    *(end + 1) = '\0';

    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);

    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }

    return value;
}