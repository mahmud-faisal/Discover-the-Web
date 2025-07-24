#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node structure for string data (used by both Stack and Queue)
typedef struct Node {
    char* data;
    struct Node* next;
    struct Node* prev;
} Node;

// Function to create a new node with string data
Node* createNode(const char* data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory error\n");
        return NULL;
    }
    newNode->data = (char*)malloc(strlen(data) + 1);
    strcpy(newNode->data, data);
    newNode->next = newNode->prev = NULL;
    return newNode;
}

/******************** QUEUE ********************/
typedef struct {
    Node* front;
    Node* rear;
} Queue;

Queue* createQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

int isEmptyQueue(Queue* q) {
    return (q->front == NULL);
}

void enqueueRear(Queue* q, const char* data) {
    Node* newNode = createNode(data);
    if (!newNode) return;

    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        newNode->prev = q->rear;
        q->rear->next = newNode;
        q->rear = newNode;
    }
    printf("current site:%s\n", data);
}

char* dequeueRear(Queue* q) {
    if (isEmptyQueue(q)) {
        printf("Nothing found to back\n");
        return NULL;
    }
    Node* temp = q->rear;
    char* data = temp->data;

    q->rear = q->rear->prev;
    if (q->rear == NULL)
        q->front = NULL;
    else
        q->rear->next = NULL;

    free(temp);
    return data;
}

void displayQueue(Queue* q) {
    if (isEmptyQueue(q)) {
        printf("History is empty\n");
        return;
    }
    Node* temp = q->front;
    // printf("Queue (front to rear): ");
    while (temp != NULL) {
        printf("%s -->", temp->data);
        temp = temp->next;
    }
    printf("\n");
}

void freeQueue(Queue* q) {
    while (!isEmptyQueue(q)) {
        char* data = dequeueRear(q);
        free(data);
    }
    free(q);
}

/******************** STACK ********************/
typedef struct {
    Node* top;
    int size;
} Stack;

Stack* createStack() {
    Stack* s = (Stack*)malloc(sizeof(Stack));
    s->top = NULL;
    s->size = 0;
    return s;
}

int isEmptyStack(Stack* s) {
    return (s->top == NULL);
}

void push(Stack* s, const char* data) {
    Node* newNode = createNode(data);
    if (!newNode) return;
    newNode->next = s->top;
    s->top = newNode;
    s->size++;
    // printf("\"%s\" pushed to stack\n", data);
}

char* pop(Stack* s) {
    if (isEmptyStack(s)) {
        printf("Nothing found to forward\n");
        return NULL;
    }
    Node* temp = s->top;
    char* data = temp->data;
    s->top = s->top->next;
    s->size--;
    free(temp);
    return data;
}

void popAll(Stack* s) {
    while (!isEmptyStack(s)) {
        char* data = pop(s);
        free(data);
    }
}

void displayStack(Stack* s) {
    if (isEmptyStack(s)) {
        printf("Stack is empty\n");
        return;
    }
    printf("Stack (top to bottom):\n");
    Node* temp = s->top;
    while (temp != NULL) {
        printf("| \"%s\" |\n", temp->data);
        temp = temp->next;
    }
    printf("|______|\n");
}

void freeStack(Stack* s) {
    popAll(s);
    free(s);
}

/******************** MAIN ********************/
int main() {
    Queue* q = createQueue();
    Stack* s = createStack();

    char buffer[100];

    while (1) {
        printf("\nEnter command or search: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = 0;  // remove newline

        if (strcmp(buffer, "exit") == 0) {
            printf("Exiting...\n");
            break;
        }
        else if (strcmp(buffer, "history") == 0) {
            printf("\n");
            displayQueue(q);
            printf("\n");
        } 
        else if (strcmp(buffer, "<") == 0) {  // Back
            char* str = dequeueRear(q);
            if (str) {
                push(s, str);
                printf("Moved back from: \"%s\"\n", str);
                free(str);
            }
        }
        else if (strcmp(buffer, ">") == 0) {  // Forward
            char* str = pop(s);
            if (str) {
                enqueueRear(q, str);
                printf("Moved forward to: \"%s\"\n", str);
                free(str);
            }
        }
        else {  // Any other string = New Search
            enqueueRear(q, buffer);
            popAll(s);  // Clear forward history
        }
    }

    freeQueue(q);
    freeStack(s);
    return 0;
}

