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

/*
 * Complete the 'timeConversion' function below.
 *
 * The function is expected to return a STRING.
 * The function accepts STRING s as parameter.
 */
char* timeConversion(char* s) {
    char *result = (char*)malloc(9 * sizeof(char));
    int hh, mm, ss;
    char am_pm[3];

    // Read the time components from the input string
    sscanf(s, "%d:%d:%d%s", &hh, &mm, &ss, am_pm);

    // Apply the conversion logic
    if (strcmp(am_pm, "PM") == 0 && hh != 12) {
        hh += 12;
    } else if (strcmp(am_pm, "AM") == 0 && hh == 12) {
        hh = 0;
    }

    // Format the result into a new string
    sprintf(result, "%02d:%02d:%02d", hh, mm, ss);
    
    return result;
}

// Provided main and readline functions are not modified
char* readline();

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    char* s = readline();

    char* result = timeConversion(s);

    fprintf(fptr, "%s\n", result);

    fclose(fptr);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = (char*)malloc(alloc_length);

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

        data = (char*)realloc(data, alloc_length);

        if (!data) {
            data = '\0';
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        data = (char*)realloc(data, data_length);

        if (!data) {
            data = '\0';
        }
    } else {
        data = (char*)realloc(data, data_length + 1);

        if (!data) {
            data = '\0';
        } else {
            data[data_length] = '\0';
        }
    }

    return data;
}