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

char* isValid(char* s) {
    int freq[256] = {0};
    for (int i = 0; s[i] != '\0'; i++) {
        freq[(unsigned char)s[i]]++;
    }

    // Count how many characters have each frequency
    // freq_count[i] = number of chars with frequency i
    int max_freq = 0;
    for (int i = 0; i < 256; i++) {
        if (freq[i] > max_freq) max_freq = freq[i];
    }
    int *freq_count = calloc(max_freq + 1, sizeof(int));
    if (!freq_count) return "NO"; // malloc failed, just say NO

    for (int i = 0; i < 256; i++) {
        if (freq[i] > 0) {
            freq_count[freq[i]]++;
        }
    }

    // Count how many distinct frequencies exist
    int distinct_freqs = 0;
    for (int i = 1; i <= max_freq; i++) {
        if (freq_count[i] > 0) distinct_freqs++;
    }

    if (distinct_freqs == 1) {
        free(freq_count);
        return "YES";
    } else if (distinct_freqs == 2) {
        int key1 = 0, key2 = 0;
        int val1 = 0, val2 = 0;
        // find the two frequencies and their counts
        for (int i = 1; i <= max_freq; i++) {
            if (freq_count[i] > 0) {
                if (key1 == 0) {
                    key1 = i;
                    val1 = freq_count[i];
                } else {
                    key2 = i;
                    val2 = freq_count[i];
                }
            }
        }
        // Case A: one frequency is 1 and it occurs once
        if ((key1 == 1 && val1 == 1) || (key2 == 1 && val2 == 1)) {
            free(freq_count);
            return "YES";
        }
        // Case B: frequencies differ by 1 and higher freq occurs once
        if ((abs(key1 - key2) == 1) &&
            ((key1 > key2 && val1 == 1) || (key2 > key1 && val2 == 1))) {
            free(freq_count);
            return "YES";
        }
    }

    free(freq_count);
    return "NO";
}

int main()
{
    char* s = readline();
    char* result = isValid(s);
    printf("%s\n", result);
    free(s);
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
