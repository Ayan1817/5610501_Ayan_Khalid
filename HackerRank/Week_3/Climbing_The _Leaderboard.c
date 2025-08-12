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
 * Complete the 'climbingLeaderboard' function below.
 *
 * The function is expected to return an INTEGER_ARRAY.
 * The function accepts following parameters:
 *  1. INTEGER_ARRAY ranked
 *  2. INTEGER_ARRAY player
 */

int* climbingLeaderboard(int ranked_count, int* ranked, int player_count, int* player, int* result_count) {
    // Remove duplicates from ranked, descending order assumed
    int* unique_ranked = malloc(ranked_count * sizeof(int));
    if (!unique_ranked) return NULL;

    int unique_count = 0;
    unique_ranked[unique_count++] = ranked[0];

    for (int i = 1; i < ranked_count; i++) {
        if (ranked[i] != ranked[i - 1]) {
            unique_ranked[unique_count++] = ranked[i];
        }
    }

    int* result = malloc(player_count * sizeof(int));
    if (!result) {
        free(unique_ranked);
        return NULL;
    }

    for (int i = 0; i < player_count; i++) {
        int score = player[i];

        // Binary search for the first unique_ranked[pos] <= score
        int left = 0, right = unique_count - 1;
        int pos = unique_count; // default position if score less than all ranks

        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (unique_ranked[mid] <= score) {
                pos = mid;
                right = mid - 1;
            } else {
                left = mid + 1;
            }
        }
        result[i] = pos + 1;
    }

    free(unique_ranked);
    *result_count = player_count;
    return result;
}

int main()
{
    int ranked_count = parse_int(ltrim(rtrim(readline())));

    char** ranked_temp = split_string(rtrim(readline()));

    int* ranked = malloc(ranked_count * sizeof(int));

    for (int i = 0; i < ranked_count; i++) {
        int ranked_item = parse_int(*(ranked_temp + i));
        *(ranked + i) = ranked_item;
    }

    int player_count = parse_int(ltrim(rtrim(readline())));

    char** player_temp = split_string(rtrim(readline()));

    int* player = malloc(player_count * sizeof(int));

    for (int i = 0; i < player_count; i++) {
        int player_item = parse_int(*(player_temp + i));
        *(player + i) = player_item;
    }

    int result_count;
    int* result = climbingLeaderboard(ranked_count, ranked, player_count, player, &result_count);

    for (int i = 0; i < result_count; i++) {
        printf("%d", *(result + i));
        if (i != result_count - 1) {
            printf("\n");
        }
    }

    printf("\n");

    free(ranked);
    free(player);
    free(result);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;

    char* data = malloc(alloc_length);
    if (!data) return NULL;

    while (true) {
        char* cursor = data + data_length;
        if (!fgets(cursor, (int)(alloc_length - data_length), stdin)) {
            break;
        }
        data_length += strlen(cursor);
        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }
        alloc_length <<= 1;
        char* temp = realloc(data, alloc_length);
        if (!temp) {
            free(data);
            return NULL;
        }
        data = temp;
    }
    if (data_length > 0 && data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';
        char* temp = realloc(data, data_length);
        if (temp) data = temp;
    } else {
        char* temp = realloc(data, data_length + 1);
        if (temp) {
            data = temp;
            data[data_length] = '\0';
        }
    }
    return data;
}

char* ltrim(char* str) {
    if (!str) {
        return NULL;
    }
    while (*str != '\0' && isspace(*str)) {
        str++;
    }
    return str;
}

char* rtrim(char* str) {
    if (!str) {
        return NULL;
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
        char** temp = realloc(splits, sizeof(char*) * (spaces + 1));
        if (!temp) {
            free(splits);
            return NULL;
        }
        splits = temp;
        splits[spaces++] = token;
        token = strtok(NULL, " ");
    }
    return splits;
}

int parse_int(char* str) {
    char* endptr;
    int value = (int)strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') {
        exit(EXIT_FAILURE);
    }
    return value;
}
