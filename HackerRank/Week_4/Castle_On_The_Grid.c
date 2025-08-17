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

typedef struct Point {
    int x, y;
} Point;

typedef struct QueueNode {
    Point pt;
    int dist;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode *front, *rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

void enqueue(Queue* q, Point pt, int dist) {
    QueueNode* temp = (QueueNode*)malloc(sizeof(QueueNode));
    temp->pt = pt;
    temp->dist = dist;
    temp->next = NULL;
    if (!q->rear) {
        q->front = q->rear = temp;
        return;
    }
    q->rear->next = temp;
    q->rear = temp;
}

QueueNode* dequeue(Queue* q) {
    if (!q->front) return NULL;
    QueueNode* temp = q->front;
    q->front = q->front->next;
    if (!q->front) q->rear = NULL;
    return temp;
}

bool isEmpty(Queue* q) {
    return q->front == NULL;
}

int minimumMoves(int grid_count, char** grid, int startX, int startY, int goalX, int goalY) {
    int n = grid_count;
    bool **visited = malloc(n * sizeof(bool*));
    for(int i=0;i<n;i++){
        visited[i] = malloc(n * sizeof(bool));
        for(int j=0;j<n;j++)
            visited[i][j] = false;
    }

    Queue* q = createQueue();
    Point start = {startX, startY};
    enqueue(q, start, 0);
    visited[startX][startY] = true;

    int dir[4][2] = {{0,1},{0,-1},{1,0},{-1,0}};

    while(!isEmpty(q)){
        QueueNode* node = dequeue(q);
        Point pt = node->pt;
        int dist = node->dist;
        free(node);

        if(pt.x == goalX && pt.y == goalY){
            for(int i=0;i<n;i++) free(visited[i]);
            free(visited);
            return dist;
        }

        for(int d=0;d<4;d++){
            int x = pt.x + dir[d][0];
            int y = pt.y + dir[d][1];
            while(x >= 0 && x < n && y >= 0 && y < n && grid[x][y]=='.'){
                if(!visited[x][y]){
                    visited[x][y] = true;
                    Point next = {x, y};
                    enqueue(q, next, dist+1);
                }
                x += dir[d][0];
                y += dir[d][1];
            }
        }
    }

    for(int i=0;i<n;i++) free(visited[i]);
    free(visited);
    return -1; // unreachable
}

char* readline();
char* ltrim(char*);
char* rtrim(char*);
char** split_string(char*);
int parse_int(char*);

int main()
{
    int n = parse_int(ltrim(rtrim(readline())));

    char** grid = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++) {
        char* grid_item = readline();
        *(grid + i) = grid_item;
    }

    char** first_multiple_input = split_string(rtrim(readline()));

    int startX = parse_int(*(first_multiple_input + 0));
    int startY = parse_int(*(first_multiple_input + 1));
    int goalX = parse_int(*(first_multiple_input + 2));
    int goalY = parse_int(*(first_multiple_input + 3));

    int result = minimumMoves(n, grid, startX, startY, goalX, goalY);

    printf("%d\n", result);

    for(int i=0;i<n;i++) free(grid[i]);
    free(grid);

    return 0;
}

// Utility functions
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
    }
    if (data[data_length - 1] == '\n') data[data_length - 1] = '\0';
    return data;
}

char* ltrim(char* str) {
    while (*str && isspace(*str)) str++;
    return str;
}

char* rtrim(char* str) {
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
    return value;
}
