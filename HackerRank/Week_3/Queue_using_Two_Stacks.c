#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

void enqueue(int);
void dequeue();
void print_ele();

int front = -1, rear = -1, size = 1000000;
int a[1000000];

int main() {

    int q, choice;
    scanf("%d", &q);
    while (q-- > 0) {
        scanf("%d", &choice);
        switch (choice) {
            case 1: {
                int ele;
                scanf("%d", &ele);
                enqueue(ele);
            } break;
            case 2: {
                dequeue();
            } break;
            case 3: {
                print_ele();
            } break;
        }
    }

    return 0;
}

void enqueue(int x) {
    // Check if queue is full
    if ((front == 0 && rear == size - 1) || (front == rear + 1)) {
        printf("Queue full\n");
        return;
    }

    // If queue is empty
    if (front == -1 && rear == -1) {
        front = rear = 0;
        a[rear] = x;
        return;
    }

    // If rear reached end, wrap around
    if (rear == size - 1)
        rear = 0;
    else
        rear++;

    a[rear] = x;
}

void dequeue() {
    // If queue is empty
    if (front == -1 && rear == -1) {
        printf("Queue empty\n");
        return;
    }

    // If queue has only one element
    if (front == rear) {
        front = rear = -1;
        return;
    }

    // If front reached end, wrap around
    if (front == size - 1)
        front = 0;
    else
        front++;
}

void print_ele() {
    if (front == -1 && rear == -1) {
        printf("Queue empty\n");
        return;
    }
    printf("%d\n", a[front]);
}
