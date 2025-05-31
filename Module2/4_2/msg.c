#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct queue{
    int priority;
    struct queue* next;
} queue;

queue* add_new(queue* head, int priority){
    queue* n = malloc(sizeof(queue));
    n->priority = priority;
    n->next = NULL;
    if (head == NULL) { return n;}
    if (head->priority > priority) {n->next = head; return n;}

    queue* element = head;
    while(element->next != NULL){
        if (element->next->priority > priority){
            n->next = element->next;
            element->next = n;
            return head;
        }else element = element->next;
    }
    element->next = n;
    return head;
}

int comprasion(int a, char* operation, int b){
    if (operation == "==") return a == b;
    if (operation == ">=") return a >= b;
    if (operation == "<=") return a <= b;
    if (operation == "!=") return a != b;
    if (operation == ">") return a > b;
    if (operation == "<") return a < b;
    return 0;
}

queue* delete_element(queue* head, int priority, int* error_code, char* operation){
    *error_code = 0;
    if (head == NULL){return NULL;}
    queue* element = head;

    if (comprasion(head->priority, operation, priority)) {
        element = head->next;
        free(head);
        return element;
    }
    while (element->next != NULL){
        if(comprasion(element->next->priority, operation, priority)){
            queue* temp = element->next->next;
            free(element->next);
            element->next = temp;
            return head;
        }else element = element->next;
    }
    *error_code = 1; //Не найден элемент
    return head;
}

void print_queue(queue* head){
    while(head != NULL){
        printf("%d\n", head->priority);
        head = head->next;
    }
}

int main(){
    
    queue* head = NULL;
    printf("\
1 - Добавление сообщения\n\
2 - удаление сообщения (первого в очереди)\n\
3 - удаление сообщения (с указанным приоритетом)\n\
4 - удаление сообщения (с приоритетом не ниже чем заданный)\n\
5 - Вывод очереди\n");
    while(1){
        int c;
        printf("Введите вариант: ");
        scanf("%d", &c);
        switch(c){
            case 1:
                printf("Введите приоритет: ");
                int first_priority;
                scanf("%d", &first_priority);
                head = add_new(head, first_priority);
                break;
            case 2:
                if (head == NULL) {printf("Очередь пуста\n"); break;}
                int error_code;
                head = delete_element(head, head->priority, &error_code, "==");
                if(error_code != 0) {printf("Элемент не найден\n"); break;}
                break;
            case 3:
                printf("Введите приоритет: ");
                int ac_priority;
                scanf("%d", &ac_priority);
                int error_code2;
                head = delete_element(head, ac_priority, &error_code2, "==");
                if(error_code2 != 0) {printf("Элемент не найден\n"); break;}
                break;
            case 4:
                printf("Введите приоритет: ");
                int less_priority;
                scanf("%d", &less_priority);
                int error_code3;
                head = delete_element(head, less_priority, &error_code3, ">=");
                if(error_code3 != 0) {printf("Элемент не найден\n"); break;}
                break;
            case 5:
                print_queue(head);
                break;
                        
        }
    }
}