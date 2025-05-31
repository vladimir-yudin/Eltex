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
    struct Person* prev;
    struct Person* next;
} Person;

void add_Place(struct Place *p){
    char buf[20];

    printf("Город: ");
    scanf("%s", buf);
    strncpy(p->City, buf, 20);

    printf("Улица: ");
    scanf("%s", buf);
    strncpy(p->Street, buf, 20);

    printf("Дом: ");
    scanf("%d", &p->HomeNumber);
}

void edit_Place(struct Place *p){
    char o;
    printf("Город: %s", p->City);
    printf("Изменить (y/n): ");
    getchar();
    scanf("%c", &o);
    if (o == 'y') {
        char buf[20];
        printf("Новое значение: ");
        scanf("%s", buf);
        strncpy(p->City, buf, 20);
    }
    
    printf("Улица: %s", p->Street);
    printf("Изменить (y/n): ");
    getchar();
    scanf("%c", &o);
    if (o == 'y') {
        char buf[20];
        printf("Новое значение: ");
        scanf("%s", buf);
        strncpy(p->Street, buf, 20);
    }

    printf("Дом: %d", p->HomeNumber);
    printf("Изменить: y/n\n");
    getchar();
    scanf("%c", &o);
    if (o == 'y') {
        printf("Новое значение: ");
        scanf("%d", &p->HomeNumber);
    }
    
}

Person* init_person(int id){
    Person* n = malloc(sizeof(Person));
    n->id = id;
    int h = 0;
    for (int i = 0; i < sizeof(c_field)/sizeof(c_field[0]); i++){
        printf("%s: ", c_field[i]);
        char buf[25];
        scanf("%s", buf);
        strncpy(n->FirstName + h, buf, i_fields[i]);
        h += i_fields[i];
    }
    add_Place(&n->WorkPlace);
    n->next = NULL;
    n->prev = NULL;
    return n;
}

Person* new_person(Person* head, Person* n){

    if (head == NULL) return n;
    if (head->id > n->id){n->next = head; head->prev = n; return n;}
    Person* element = head;
    while(element->next != NULL){
        if (element->next->id > n->id){
            n->prev = element;
            n->next = element->next;
            element->next->prev = n;
            element->next = n;
            return head;
        }
        element = element->next;
    }
    element->next = n;
    n->prev = element;
    return head;
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

Person* edit_person(Person* head, int id, int* error_code){
    *error_code = 0;
    Person* element = head;
    while(element != NULL){
        if (element->id == id){
            int h = 0;
            for (int i = 0; i < sizeof(c_field)/sizeof(c_field[0]); i ++){
                printf("Текущее %s: %s\n", c_field[i], element->FirstName + h);
                printf("Изменить (y/n): ");
                getchar();
                char o;
                scanf("%c", &o);
                if (o == 'y') { 
                    printf("Новое значение: ");
                    char buf[25];
                    scanf("%s", buf);
                    strncpy(element->FirstName + h, buf, i_fields[i]);
                }
                h += i_fields[i];
            }
            edit_Place(&element->WorkPlace);
            return head;
        }
        element = element->next;
    }
    error_code = 0; // Элемент не найден
    return head;
}

void print_person(Person* p){
    int h = 0;
    printf("id: %d\n", p->id);
    for (int j = 0; j < sizeof(c_field)/sizeof(c_field[0]); j++){
        printf("%s: %s\n", c_field[j], p->FirstName + h);
        h += i_fields[j];
    }
    printf("Место работы: г. %s, ул. %s, %d\n", p->WorkPlace.City, p->WorkPlace.Street, p->WorkPlace.HomeNumber);
}

void print_list(Person* head){
    while(head != NULL){
        print_person(head);
        head = head->next;
    }
}

int main(){
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
                printf("Количество тестовых записей: ");
                int number_of_testers;
                scanf("%d", &number_of_testers);
                
                int ids[] = {5, 1, 0, 4, 6, 2, 7, 9, 3, 10};
                for (int i = 0; i < number_of_testers; i++){
                    head = new_person(head, &people[i]);
                }
        }
    }
}