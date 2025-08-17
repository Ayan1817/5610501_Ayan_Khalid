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
 * Complete the 'highestValuePalindrome' function below.
 */
char* highestValuePalindrome(char* s, int n, int k) {
    char* arr = malloc((n + 1) * sizeof(char));
    strcpy(arr, s);

    bool* changeArr = calloc(n, sizeof(bool));

    int min = 0;
    int max = n - 1;

    // Step 1: Make it a palindrome with minimum changes
    while (max >= min) {
        if (arr[min] != arr[max]) {
            char maxChar = (arr[min] > arr[max]) ? arr[min] : arr[max];
            arr[min] = arr[max] = maxChar;
            k--;
            if (k < 0) {
                free(changeArr);
                free(arr);
                char* result = malloc(3);
                strcpy(result, "-1");
                return result;
            }
            changeArr[min] = changeArr[max] = true;
        }
        min++;
        max--;
    }

    // Step 2: Maximize palindrome value
    min = 0;
    max = n - 1;
    while (k > 0 && min < max) {
        if (arr[min] != '9') {
            if (changeArr[min] || changeArr[max]) {
                arr[min] = arr[max] = '9';
                k--;
            } else if (k >= 2) {
                arr[min] = arr[max] = '9';
                k -= 2;
            }
        }
        min++;
        max--;
    }

    // Step 3: If odd length, change middle element
    if (k > 0 && n % 2 != 0) {
        int mid = n / 2;
        arr[mid] = '9';
        k--;
    }

    free(changeArr);
    return arr;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char** first_multiple_input = split_string(rtrim(readline()));

    int n = parse_int(*(first_multiple_input + 0));
    int k = parse_int(*(first_multiple_input + 1));

    char* s = readline();

    char* result = highestValuePalindrome(s, n, k);

    fprintf(fptr, "%s\n", result);

    free(result);
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
    } else {
        data = realloc(data, data_length + 1);
        data[data_length] = '\0';
    }

    return data;
}

char* ltrim(char* str) {
    if (!str || !*str) return str;
    while (*str && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
    if (!str || !*str) return str;
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
