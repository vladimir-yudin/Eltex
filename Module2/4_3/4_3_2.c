#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const char* c_field[] ={
    "Имя",
    "Фамилия",
    "Отчество",
    "Должность",
    "E-mail",
    "Номер телефона",
    "Ссылка на соц. сеть",
    "Другая ссылка на соц. сеть"
};

const int i_fields[] = {20, 20, 20, 20, 20, 12, 25, 25};

struct Place {
    char City[20], Street[20];
    int HomeNumber;
};

typedef struct Person{
    int id;
    char FirstName[20], Surname[20], LastName[20], JobTitle[20], Email[20];
    char PhoneNumber[12];
    char Link1[25], Link2[25];
    struct Place WorkPlace;
    
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

Person* delete_person(Person* head, int id, int* error_code){
    *error_code = 0;
    if (head == NULL) return NULL;
    Person* element = head;

    if (head->id == id) {
        element = head->next;
        if(element != NULL) element->prev = NULL;
        //free(head);
        return element;
    }
    while (element->next != NULL){
        if(element->next->id == id){
            Person* temp = element->next->next;
            //free(element->next);
            element->next = temp;
            return head;
        }
        element = element->next;
    }
    *error_code = 1; //Не найден элемент
    return head;
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
                Person* n = init_person(id);
                head = new_person(head, n);
                break;
            case 2:
                printf("Введите id: ");
                scanf("%d", &id);\
                head = delete_person(head, id, &error_code);
                if(error_code != 0) {printf("Элемент не найден\n"); break;}
                break;
            case 3:
                printf("Введите id: ");
                scanf("%d", &id);
                head = edit_person(head, id, &error_code);
                if(error_code != 0) {printf("Элемент не найден\n"); break;}
                break;
            case 4:
                print_list(head);
                break;
            case 5:
                printf("qdqwd\n");
                Person people[10] = {
                    {1,"Konstantin", "Petrov", "Igorevich", "Plumber", "petrov1978@gmail.com", 
                     "89645672314", "vk.com/213125435353", "@petrov_tg", {"Novosibirsk", "Titova", 15}, NULL, NULL},
                    {5, "Anna", "Ivanova", "Sergeevna", "Engineer", "anna.ivan@mail.com", 
                     "89536781245", "linkedin.com/in/ivanova", "@anna_iv", {"Moscow", "Tverskaya", 7}, NULL, NULL},
                    {7, "Dmitry", "Sokolov", "Alexeevich", "Teacher", "dsokolov@yandex.ru", 
                     "89234567890", "facebook.com/dsokolov", "@sokolov_d", {"Saint-Petersburg", "Nevsky", 28}, NULL, NULL},
                    {8, "Elena", "Smirnova", "Vladimirovna", "Doctor", "elena_smirn@mail.ru", 
                     "89015673421", "ok.ru/elenasmirnova", "@smirnova_e", {"Kazan", "Baumana", 11}, NULL, NULL},
                    {3, "Alexey", "Kuznetsov", "Nikolaevich", "Programmer", "akuznetsov@gmail.com", 
                     "89123456789", "github.com/kuznetsov", "@kuznetsov_a", {"Yekaterinburg", "Lenina", 45}, NULL, NULL},
                    {2, "Marina", "Popova", "Fedorovna", "Designer", "marinapopova@mail.ru", 
                     "89217654321", "behance.net/popova", "@popova_m", {"Rostov-on-Don", "Gorkogo", 3}, NULL, NULL},
                    {10, "Sergey", "Orlov", "Evgenievich", "Driver", "sergey.orlov@bk.ru", 
                     "89567890123", "vk.com/orlovsergey", "@orlov_s", {"Omsk", "Lenina", 20}, NULL, NULL},
                    {11, "Natalia", "Vasilieva", "Olegovna", "Lawyer", "n.vasilieva@list.ru", 
                     "89876543210", "facebook.com/vasilieva", "@vasilieva_n", {"Samara", "Kuybysheva", 12}, NULL, NULL},
                    {4, "Victor", "Morozov", "Stepanovich", "Firefighter", "vicmorozov@yandex.ru", 
                     "89098765432", "ok.ru/morozov", "@morozov_v", {"Chelyabinsk", "Kirova", 9}, NULL, NULL},
                    {15, "Olga", "Nikiforova", "Artemovna", "Accountant", "olganiki@gmail.com", 
                     "89991234567", "linkedin.com/in/nikiforov", "@nikiforova_o", {"Vladivostok", "Svetlanskaya", 33}, NULL, NULL}
                };
                for (int i = 0; i < sizeof(people)/sizeof(people[0]); i++){
                    new_person(head, &people[i]);
                }
        }
    }
}