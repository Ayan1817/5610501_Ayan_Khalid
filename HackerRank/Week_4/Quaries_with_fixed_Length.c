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

/* Deque structure for indices */
typedef struct {
    int* data;
    int front;
    int back;
    int capacity;
} Deque;

Deque* create_deque(int size) {
    Deque* dq = malloc(sizeof(Deque));
    dq->data = malloc(size * sizeof(int));
    dq->front = 0;
    dq->back = -1;
    dq->capacity = size;
    return dq;
}

void deque_push_back(Deque* dq, int value) {
    dq->back++;
    dq->data[dq->back] = value;
}

void deque_pop_back(Deque* dq) {
    dq->back--;
}

void deque_pop_front(Deque* dq) {
    dq->front++;
}

int deque_front(Deque* dq) {
    return dq->data[dq->front];
}

bool deque_empty(Deque* dq) {
    return dq->front > dq->back;
}

void free_deque(Deque* dq) {
    free(dq->data);
    free(dq);
}

/* Function to find min of maximums in all subarrays of size d */
int min_of_max_in_subarrays_with_size(int d, int* arr, int arr_count) {
    Deque* dq = create_deque(arr_count);
    int* candidates = malloc(arr_count * sizeof(int));
    int c_index = 0;

    for (int i = 0; i < arr_count; i++) {
        if (!deque_empty(dq) && deque_front(dq) <= i - d) {
            deque_pop_front(dq);
        }
        while (!deque_empty(dq) && arr[dq->data[dq->back]] <= arr[i]) {
            deque_pop_back(dq);
        }
        deque_push_back(dq, i);

        if (i >= d - 1) {
            candidates[c_index++] = arr[deque_front(dq)];
        }
    }

    int min_val = INT_MAX;
    for (int i = 0; i < c_index; i++) {
        if (candidates[i] < min_val) min_val = candidates[i];
    }

    free(candidates);
    free_deque(dq);
    return min_val;
}

/* Solve function for all queries */
int* solve(int arr_count, int* arr, int queries_count, int* queries, int* result_count) {
    int* results = malloc(queries_count * sizeof(int));
    *result_count = queries_count;

    for (int i = 0; i < queries_count; i++) {
        results[i] = min_of_max_in_subarrays_with_size(queries[i], arr, arr_count);
    }
    return results;
}

int main() {
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));
    int n = parse_int(*(first_multiple_input + 0));
    int q = parse_int(*(first_multiple_input + 1));

    char** arr_temp = split_string(rtrim(readline()));
    int* arr = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        arr[i] = parse_int(arr_temp[i]);
    }

    int* queries = malloc(q * sizeof(int));
    for (int i = 0; i < q; i++) {
        queries[i] = parse_int(ltrim(rtrim(readline())));
    }

    int result_count;
    int* result = solve(n, arr, q, queries, &result_count);

    for (int i = 0; i < result_count; i++) {
        fprintf(fptr, "%d", result[i]);
        if (i != result_count - 1) fprintf(fptr, "\n");
    }
    fprintf(fptr, "\n");

    fclose(fptr);
    free(arr);
    free(queries);
    free(result);

    return 0;
}

/* Utility functions for I/O */
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
