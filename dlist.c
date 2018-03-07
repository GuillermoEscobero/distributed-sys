/* Doubly Linked List implementation */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 256

/* Definition of a node of the data structure */
typedef struct Node  {
    int key;
    char value1[MAX_SIZE];
    float value2;
    struct Node* next;
    struct Node* prev;
} Node;

/* Head of the Doubly Linked List*/
Node* head;

int init() {
    if(head == NULL) return 0;

    Node* node = head;
    Node* temp;
    while(node != NULL) {
        temp = node;
        node = node->next;
        free(temp);
    }

    head = NULL;

    return 0;
}

/* Method to create a new node */
Node* getNewNode(int key, char *value1, float value2) {
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->key = key;
    strcpy(newNode->value1, value1);
    newNode->value2 = value2;
    newNode->prev = NULL;
    newNode->next = NULL;

    return newNode;
}

/* Method to print in stdout the current nodes and their values (debugging) */
void printList() {
	Node* temp = head;
  int counter = 0;
	while(temp != NULL) {
		printf("NODE %d: %d, %s, %f\n", counter, temp->key, temp->value1, temp->value2);
		temp = temp->next;
        counter++;
	}
}

/* Method to insert a new node in the list. This node will be added at the end */
int insert(Node* newNode, int key) {
    Node* temp = head;

    if(head == NULL) {
        head = newNode;
        printf("INSERTED IN HEAD\n");
        return 0;
    }

    while(temp != NULL) {
        printf("A VER QUE POLLAS PASA:::%d,,,%d\n", temp->key, newNode->key);
        if(temp->key == key) {
            printf("COJONEEEEEEEEEEESSSSS\neeeeeeeeeee\n\n\n");
            /* Key already exists */
            printList();
            return -1;
        }

        if(temp->next != NULL) {
            temp = temp->next;
        } else {
            break;
        }
    }

    temp->next = newNode;
    newNode->prev = temp;
    return 0;
}

/* Deletes the node with the provided key */
int delete(int key) {
    Node* temp = head;

    if(head == NULL) {
        return -1;
    }

    while(temp->next != NULL) {
        if(temp->key == key) {
            if(temp == head) {
                head = temp->next;
                free(temp);
                return 0;
            }

            Node* prev = temp->prev;
            Node* next = temp->next;

            prev->next = temp->next;
            next->prev = temp->prev;

            free(temp);

            return 0;
        }
        temp = temp->next;
    }

    return -1;
}

/* Returns the node with the provided key associated */
Node* search(int key) {
    Node* temp = head;

    if(head == NULL) {
        return NULL;
    }

    while(temp != NULL) {
        printf("A VER QUE POLLAS PASA:::%d,,,%d\n", temp->key, key);
        if(temp->key == key) {
            return temp;
        }
	      temp = temp->next;
    }

    return NULL;
}

int modify(Node* newNode) {
    Node* temp = head;

    if(head == NULL) {
        return -1;
    }

    while(temp != NULL) {
        printf("A VER QUE POLLAS PASA:::%d,,,%d\n", temp->key, newNode->key);
        if(temp->key == newNode->key) {
            temp = newNode;
            return 0;
        }
	      temp = temp->next;
    }

    return -1;
}

int getCardinality() {
    Node* temp = head;

    int count = 0;

    if(head == NULL) {
        return 0;
    }

    while(temp != NULL) {
        count++;
    	temp = temp->next;
    }

    return count;
}
