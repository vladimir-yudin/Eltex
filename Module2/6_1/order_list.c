#include "order_list.h"

Node* init_node(int key, void* data){
    Node* new_node = malloc(sizeof(Node));
    new_node->key = key;
    new_node->data = data;
    new_node->next = NULL;
    new_node->prev = NULL;
    return new_node;
}

Node* search_node(Node* head, int key){
    while(head != NULL){
        if (head->key == key){
            return head;
        }
        head = head->next;
    }
    return NULL;
}

int add_node(Node** head, int key, void* data){
    Node* new_node = init_node(key, data);
    if ((*head) == NULL) { (*head) = new_node; return 0;}
    if ((*head)->key > new_node->key){
        new_node->next = (*head); 
        (*head)->prev = new_node; 
        (*head) = new_node; 
        return 0;
    }
    Node* element = *head;
    while(element->next != NULL){
        if (element->next->key > new_node->key){
            new_node->prev = element;
            new_node->next = element->next;
            element->next->prev = new_node;
            element->next = new_node;
            return 0;
        }
        element = element->next;
    }
    element->next = new_node;
    new_node->prev = element;
    return 0;
}

int delete_node(Node ** head, int key){
    if ((*head) == NULL) return 0;
    Node* element = *head;

    if ((*head)->key == key) {
        *head = (*head)->next;
        free(element);
        return 0;
    }
    while (element->next != NULL){
        if(element->next->key == key){
            Node* temp = element->next;
            element->next = temp->next;
            temp->prev = element;
            free(temp);
            return 0;
        }
        element = element->next;
    }
    return -1;
}