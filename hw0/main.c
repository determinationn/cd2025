#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    char character;
    int count;
    struct Node* next;
} Node;

Node* createNode(char character) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    newNode->character = character;
    newNode->count = 1;
    newNode->next = NULL;
    return newNode;
}

void insertOrUpdate(Node** head, char character) {
    if (character < 32 || character > 126 || character == ' ') { 
        return;
    }

    Node* current = *head;
    Node* prev = NULL;
    
    while (current) {
        if (current->character == character) {
            current->count++;
            return;
        }
        prev = current;
        current = current->next;
    }
    
    Node* newNode = createNode(character);
    if (prev) {
        prev->next = newNode;
    } else {
        *head = newNode;
    }
}

void freeList(Node* head) {
    Node* temp;
    while (head) {
        temp = head;
        head = head->next;
        free(temp);
    }
}

void printList(Node* head) {
    Node* current = head;
    while (current) {
        printf("%c : %d\n", current->character, current->count);
        current = current->next;
    }
}

int main() {
    FILE *file = fopen("main.c", "r");
    if (!file) {
        printf("Failed to open file!\n");
        return 1;
    }
    
    Node* head = NULL;
    char ch;
    
    while ((ch = fgetc(file)) != EOF) {
        insertOrUpdate(&head, ch);
    }
    
    fclose(file);
    printList(head);
    freeList(head);
    return 0;
}