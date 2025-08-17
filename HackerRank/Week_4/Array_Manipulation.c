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

/*
 * Complete the 'arrayManipulation' function below.
 *
 * The function is expected to return a LONG_INTEGER.
 * The function accepts following parameters:
 *  1. INTEGER n
 *  2. 2D_INTEGER_ARRAY queries
 */
long arrayManipulation(int n, int queries_rows, int queries_columns, int** queries) {
    long* arr = calloc(n + 1, sizeof(long));
    
    for (int i = 0; i < queries_rows; i++) {
        int a = queries[i][0] - 1; // 0-based index
        int b = queries[i][1];     // b+1 index for subtraction
        int k = queries[i][2];
        
        arr[a] += k;
        if (b < n) arr[b] -= k;
    }

    long max = 0;
    long running = 0;
    for (int i = 0; i < n; i++) {
        running += arr[i];
        if (running > max) max = running;
    }

    free(arr);
    return max;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));

    int m = parse_int(*(first_multiple_input + 1));

    int** queries = malloc(m * sizeof(int*));

    for (int i = 0; i < m; i++) {
        *(queries + i) = malloc(3 * sizeof(int));

        char** queries_item_temp = split_string(rtrim(readline()));

        for (int j = 0; j < 3; j++) {
            int queries_item = parse_int(*(queries_item_temp + j));
            *(*(queries + i) + j) = queries_item;
        }
    }

    long result = arrayManipulation(n, m, 3, queries);

    fprintf(fptr, "%ld\n", result);

    fclose(fptr);

    for (int i = 0; i < m; i++) free(queries[i]);
    free(queries);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) break;

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;

        alloc_length <<= 1;
        data = realloc(data, alloc_length);

        if (!data) { data = "\0"; break; }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
        if (!data) data = "\0";
    } else {
        data = realloc(data, data_length + 1);
        if (!data) data = "\0"; else data[data_length] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) return "\0";
    while (*str && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return "\0";
    char* end = str + strlen(str) - 1;
    while (end >= str && isspace(*end)) end--;
    *(end + 1) = '\0';
    return str;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");
    int spaces = 0;
    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);
        splits[spaces - 1] = token;
        token = strtok(NULL, " ");
    }
    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}
