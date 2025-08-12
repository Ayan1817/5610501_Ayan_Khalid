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

bool isPrime(int num) {
    if (num < 2) return false;
    int root = (int)sqrt(num);
    for (int i = 2; i <= root; i++) {
        if (num % i == 0) return false;
    }
    return true;
}

int* generatePrimes(int q) {
    int* primes = malloc(q * sizeof(int));
    int count = 0;
    int num = 2;
    while (count < q) {
        if (isPrime(num)) {
            primes[count++] = num;
        }
        num++;
    }
    return primes;
}

int* waiter(int number_count, int* number, int q, int* result_count) {
    int* primes = generatePrimes(q);

    // Allocate stacks A and B for each iteration
    int** A = malloc((q + 1) * sizeof(int*));
    int** B = malloc((q + 1) * sizeof(int*));
    int* A_size = calloc(q + 1, sizeof(int));
    int* B_size = calloc(q + 1, sizeof(int));

    for (int i = 0; i <= q; i++) {
        A[i] = malloc(number_count * sizeof(int));
        B[i] = malloc(number_count * sizeof(int));
    }

    // Initialize B0 with input numbers in order
    for (int i = 0; i < number_count; i++) {
        B[0][i] = number[i];
    }
    B_size[0] = number_count;
    A_size[0] = 0;

    int* output = malloc(number_count * sizeof(int));
    int output_index = 0;

    for (int i = 1; i <= q; i++) {
        int prime = primes[i - 1];
        A_size[i] = 0;
        B_size[i] = 0;

        // Process B[i-1] from top to bottom (end to start), to simulate stack pop
        for (int j = B_size[i - 1] - 1; j >= 0; j--) {
            int val = B[i - 1][j];
            if (val % prime == 0) {
                A[i][A_size[i]++] = val;  // divisible go to A[i] stack
            } else {
                B[i][B_size[i]++] = val;  // non-divisible go to B[i]
            }
        }

        // Pop all from A[i] in order (top to bottom)
        for (int k = A_size[i] - 1; k >= 0; k--) {
            output[output_index++] = A[i][k];
        }
    }

    // After q iterations, pop all remaining elements from B[q] in reverse order
    for (int k = B_size[q] - 1; k >= 0; k--) {
        output[output_index++] = B[q][k];
    }

    // Free memory
    for (int i = 0; i <= q; i++) {
        free(A[i]);
        free(B[i]);
    }
    free(A);
    free(B);
    free(A_size);
    free(B_size);
    free(primes);

    *result_count = output_index;
    return output;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));
    int q = parse_int(*(first_multiple_input + 1));

    char** number_temp = split_string(rtrim(readline()));

    int* number = malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        int number_item = parse_int(*(number_temp + i));
        *(number + i) = number_item;
    }

    int result_count;
    int* result = waiter(n, number, q, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", *(result + i));
        if (i != result_count - 1) {
            fprintf(fptr, "\n");
        }
    }

    fprintf(fptr, "\n");

    fclose(fptr);
    free(number);
    free(result);

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

        size_t new_length = alloc_length << 1;
        data = realloc(data, new_length);
        if (!data) break;
        alloc_length = new_length;
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = realloc(data, data_length);
        if (!data) data = '\0';
    } else {
        data = realloc(data, data_length + 1);
        if (!data) data = '\0';
        else data[data_length] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
    while (*str != '\0' && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str) return '\0';
    if (!*str) return str;
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
        if (!splits) return splits;
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
