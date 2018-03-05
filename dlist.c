/* Doubly Linked List implementation */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 256

typedef struct Node  {
    int key;
    char value1[MAX_SIZE];
    float value2;
    struct Node* next;
    struct Node* prev;
} Node;

Node* head; // global variable - pointer to head node.

Node* getNewNode(int key, char *value1, float value2) {
    Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->key = key;
    strcpy(newNode->value1, value1);
    newNode->value2 = value2;
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

int insert(Node* newNode, Node* head) {
    Node* temp = head;

    if(head == NULL) {
        head = newNode;
        return 0;
    }

    while(temp->next != NULL) {
        if(temp->key == newNode->key) {
            return -1;
        }
        temp = temp->next;
    }

    temp->next = newNode;
    newNode->prev = temp;

    return 0;
}

int delete(int key, Node* head) {
    Node* temp = head;

    if(head == NULL) {
        return -1;
    }

    while(temp->next != NULL) {
        if(temp->key == key) {
            Node* prev = temp->prev;
            Node* next = temp->next;

            prev->next = temp->next;
            next->prev = temp->prev;

            free(temp);

            return 0;
        }
    }

    return -1;
}

Node* search(int key, Node* head) {
    Node* temp = head;

    if(head == NULL) {
        return NULL;
    }

    while(temp->next != NULL) {
        if(temp->key == key) {
            return temp;
        }
    }

    return NULL;
}

int modify(Node* newNode, Node* head) {
    Node* temp = head;

    if(head == NULL) {
        return -1;
    }

    while(temp->next != NULL) {
        if(temp->key == newNode->key) {
            temp = newNode;
            return 0;
        }
    }

    return -1;
}

int getCardinality(Node* head) {
    Node* temp = head;

    int count = 0;

    if(head == NULL) {
        return 0;
    }

    while(temp->next != NULL) {
        count++;
    }

    return count;
}
