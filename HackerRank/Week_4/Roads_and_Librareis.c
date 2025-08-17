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

/* Linked list node for adjacency */
typedef struct Node {
    int val;
    struct Node* next;
} Node;

/* Add edge to adjacency list */
void add_edge(Node** adj, int u, int v) {
    Node* new_node = malloc(sizeof(Node));
    new_node->val = v;
    new_node->next = adj[u];
    adj[u] = new_node;
}

/* DFS to count nodes in connected component */
long dfs(int u, Node** adj, bool* visited) {
    visited[u] = true;
    long count = 1; // count this node
    Node* curr = adj[u];
    while (curr) {
        if (!visited[curr->val]) {
            count += dfs(curr->val, adj, visited);
        }
        curr = curr->next;
    }
    return count;
}

/* Main function */
long roadsAndLibraries(int n, int c_lib, int c_road, int cities_rows, int cities_columns, int** cities) {
    if (c_road >= c_lib || cities_rows == 0)
        return 1L * n * c_lib;

    Node** adj = calloc(n, sizeof(Node*));

    for (int i = 0; i < cities_rows; i++) {
        int u = cities[i][0] - 1; // convert to 0-indexed
        int v = cities[i][1] - 1;
        add_edge(adj, u, v);
        add_edge(adj, v, u);
    }

    bool* visited = calloc(n, sizeof(bool));
    long total_cost = 0;

    for (int i = 0; i < n; i++) {
        if (!visited[i]) {
            long nodes_in_component = dfs(i, adj, visited);
            total_cost += c_lib + (nodes_in_component - 1) * (long)c_road;
        }
    }

    /* Free memory */
    for (int i = 0; i < n; i++) {
        Node* curr = adj[i];
        while (curr) {
            Node* tmp = curr;
            curr = curr->next;
            free(tmp);
        }
    }
    free(adj);
    free(visited);

    return total_cost;
}

int main()
{
    FILE* fptr = fopen(getenv("OUTPUT_PATH"), "w");

    int q = parse_int(ltrim(rtrim(readline())));

    for (int q_itr = 0; q_itr < q; q_itr++) {
        char** first_multiple_input = split_string(rtrim(readline()));

        int n = parse_int(*(first_multiple_input + 0));
        int m = parse_int(*(first_multiple_input + 1));
        int c_lib = parse_int(*(first_multiple_input + 2));
        int c_road = parse_int(*(first_multiple_input + 3));

        int** cities = malloc(m * sizeof(int*));
        for (int i = 0; i < m; i++) {
            *(cities + i) = malloc(2 * sizeof(int));
            char** cities_item_temp = split_string(rtrim(readline()));
            for (int j = 0; j < 2; j++) {
                int cities_item = parse_int(*(cities_item_temp + j));
                *(*(cities + i) + j) = cities_item;
            }
        }

        long result = roadsAndLibraries(n, c_lib, c_road, m, 2, cities);
        fprintf(fptr, "%ld\n", result);

        /* Free cities memory */
        for (int i = 0; i < m; i++) free(cities[i]);
        free(cities);
    }

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

        if (!line) break;

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') break;

        alloc_length <<= 1;
        data = realloc(data, alloc_length);
        if (!data) { data = "\0"; break; }
    }

    if (data[data_length - 1] == '\n') data[data_length - 1] = '\0';
    else data = realloc(data, data_length + 1);

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
