#ifndef ORDERED_LIST_H
#define ORDERED_LIST_H

#define NO_ELEMENT -1

#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    void *data;
    struct Node *prev;
    struct Node *next;
} Node;

Node* init_node(int key, void* data);

Node* search_node(Node* head, int key);

int add_node(Node** head, int key, void* data);

int delete_node(Node ** head, int key);

#endif