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

#define ALPHABET_SIZE 26

typedef struct TrieNode {
    bool end_word;
    struct TrieNode* children[ALPHABET_SIZE];
} TrieNode;

TrieNode* createNode() {
    TrieNode* node = (TrieNode*)malloc(sizeof(TrieNode));
    node->end_word = false;
    for (int i = 0; i < ALPHABET_SIZE; i++) node->children[i] = NULL;
    return node;
}

bool insert(TrieNode* root, char* word) {
    TrieNode* cur = root;
    for (int i = 0; word[i]; i++) {
        int index = word[i] - 'a';
        if (!cur->children[index]) {
            cur->children[index] = createNode();
        } else {
            if (cur->children[index]->end_word) {
                // current prefix is already a word
                return true;
            }
        }
        cur = cur->children[index];
    }
    // if cur has any children, then current word is prefix of another
    for (int i = 0; i < ALPHABET_SIZE; i++) {
        if (cur->children[i]) return true;
    }
    cur->end_word = true;
    return false;
}

void noPrefix(int words_count, char** words) {
    TrieNode* root = createNode();
    for (int i = 0; i < words_count; i++) {
        if (insert(root, words[i])) {
            printf("BAD SET\n%s\n", words[i]);
            return;
        }
    }
    printf("GOOD SET\n");
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
        if (!data) { data = '\0'; break; }
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

int parse_int(char* str) {
    char* endptr;
    int value = strtol(str, &endptr, 10);
    if (endptr == str || *endptr != '\0') exit(EXIT_FAILURE);
    return value;
}

int main() {
    int n = parse_int(ltrim(rtrim(readline())));
    char** words = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        char* words_item = readline();
        *(words + i) = words_item;
    }
    noPrefix(n, words);
    return 0;
}
