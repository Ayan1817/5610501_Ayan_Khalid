#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {

    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int Q;
scanf("%d", &Q);

int heap[100000]; // Max size, adjust if needed
int size = 0;

// Helper functions
void swap(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

void heapifyUp(int i) {
    while (i > 0) {
        int parent = (i - 1) / 2;
        if (heap[i] < heap[parent]) {
            swap(&heap[i], &heap[parent]);
            i = parent;
        } else break;
    }
}

void heapifyDown(int i) {
    while (1) {
        int left = 2*i + 1, right = 2*i + 2, smallest = i;
        if (left < size && heap[left] < heap[smallest]) smallest = left;
        if (right < size && heap[right] < heap[smallest]) smallest = right;
        if (smallest != i) {
            swap(&heap[i], &heap[smallest]);
            i = smallest;
        } else break;
    }
}

void insert(int val) {
    heap[size] = val;
    size++;
    heapifyUp(size - 1);
}

void removeVal(int val) {
    for (int i = 0; i < size; i++) {
        if (heap[i] == val) {
            heap[i] = heap[size - 1];
            size--;
            heapifyDown(i);
            heapifyUp(i);
            break;
        }
    }
}

int peek() {
    return heap[0];
}

// Process queries
for (int i = 0; i < Q; i++) {
    int type, x;
    scanf("%d", &type);
    switch(type) {
        case 1:
            scanf("%d", &x);
            insert(x);
            break;
        case 2:
            scanf("%d", &x);
            removeVal(x);
            break;
        case 3:
            if (size > 0)
                printf("%d\n", peek());
            break;
        default:
            printf("Invalid !\n");
    }
}
    
    return 0;
}
