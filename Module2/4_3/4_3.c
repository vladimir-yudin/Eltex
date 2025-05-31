#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct Person{
    int id;

    int balance_factor;
    struct Person* left;
    struct Person* right;
} Person;

Person* init_person(int id){
    Person* n = malloc(sizeof(Person));
    n->id = id;
    
    n->balance_factor = 0;
    n->left = NULL;
    n->right = NULL;
}

Person* balance_tree(Person* head){
    if (abs(head->balance_factor) < 2) return head;
    Person* element = head;
    if (head->balance_factor > 1){
        element = element->right;
        element->balance_factor += 1;
        while (element->left != NULL){
            element = element->left;
            
        }
    }
}

Person* new_person(Person* head, int id, int* error_code){
    *error_code = 0;
    Person* n = init_person(id);
    if (head == NULL) return n;

    Person* element = head;
    while(1){
        if(id > element->id){
            element->balance_factor += 1;
            if(element->right == NULL){
                element->right = n;
                return head;
            }
            element = element->right;
            continue;
        }
        if(id < element->id){
            element->balance_factor -= 1;
            if(element->left == NULL){
                element->left = n;
                return head;
            }
            element = element->left;
            continue;
        }
        *error_code = 1;
        return head;
    }
}

void print_person(Person* element){
    printf("%d\n", element->id);
}

void print_tree(Person* head){
    if(head == NULL) return;
    print_tree(head->left);
    print_person(head);
    print_tree(head->right);
}

int main(){
    Person* head = NULL;
    printf("\
1 - Добавление сообщения\n\
2 - Удаление записи\n\
3 - Редактирование записи\n\
4 - Вывод очереди\n\
5 - Тестовый набор данных\n");
    while(1){
        int c, id, error_code;
        printf("Введите вариант: ");
        scanf("%d", &c);
        switch(c){
            case 1:
                printf("Введите id: ");
                scanf("%d", &id);
                head = new_person(head, id, &error_code);
                break;
            case 4:
                print_tree(head);
        }
    }
}