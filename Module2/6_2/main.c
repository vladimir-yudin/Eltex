#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#define NO_ELEMENT -1

typedef void* Node;

Node (*init_node)(int, void*);
Node (*search_node)(Node, int);
int (*add_node)(Node*, int, void*);
int (*delete_node)(Node*, int);
int (*get_key)(Node*);
void* (*get_data)(Node*);
Node (*get_next)(Node*);
Node (*get_prev)(Node*);

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

const int i_fields[] = {20, 20, 20, 20, 25, 12, 25, 25};

struct Place {
    char City[20], Street[20];
    int HomeNumber;
};

typedef struct Person{
    char FirstName[20], Surname[20], LastName[20], JobTitle[20], Email[25];
    char PhoneNumber[12];
    char Link1[25], Link2[25];
    struct Place WorkPlace;
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

void edit_person(Person *p){
    int h = 0;
    for (int i = 0; i < sizeof(c_field)/sizeof(c_field[0]); i ++){
        printf("Текущее %s: %s\n", c_field[i], p->FirstName + h);
        printf("Изменить (y/n): ");
        getchar();
        char o;
        scanf("%c", &o);
        if (o == 'y') { 
            printf("Новое значение: ");
            char buf[25];
            scanf("%s", buf);
            strncpy(p->FirstName + h, buf, i_fields[i]);
        }
        h += i_fields[i];
    }
    edit_Place(&p->WorkPlace);
}

Person* init_person(){
    Person* n = malloc(sizeof(Person));
    int h = 0;
    for (int i = 0; i < sizeof(c_field)/sizeof(c_field[0]); i++){
        printf("%s: ", c_field[i]);
        char buf[25];
        scanf("%s", buf);
        strncpy(n->FirstName + h, buf, i_fields[i]);
        h += i_fields[i];
    }
    add_Place(&n->WorkPlace);
    return n;
}

void print_person(int key, Person* p){
    int h = 0;
    printf("\033[32mkey: %d\033[0m\n", key);
    for (int j = 0; j < sizeof(c_field)/sizeof(c_field[0]); j++){
        printf("%s: %s\n", c_field[j], p->FirstName + h);
        h += i_fields[j];
    }
    printf("Место работы: г. %s, ул. %s, %d\n", p->WorkPlace.City, p->WorkPlace.Street, p->WorkPlace.HomeNumber);
}

void print_list(Node* head){
    while(head != NULL){
        print_person(get_key(head), get_data(head));
        head = get_next(head);
    }
}

int main(){
    void* handle = dlopen("./liblist.so", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Ошибка загрузки: %s\n", dlerror());
        return 1;
    }
    init_node = dlsym(handle, "init_node");
    search_node = dlsym(handle, "search_node");
    add_node = dlsym(handle, "add_node");
    delete_node = dlsym(handle, "delete_node");
    get_key = dlsym(handle, "get_key");
    get_data = dlsym(handle, "get_data");
    get_next = dlsym(handle, "get_next");
    get_prev = dlsym(handle, "get_prev");
    Node head = NULL;
    Person people[10] = {
        {"Konstantin", "Petrov", "Igorevich", "Plumber", "petrov1978@gmail.com", 
        "89645672314", "vk.com/213125435353", "@petrov_tg", {"Novosibirsk", "Titova", 15}},
        {"Anna", "Ivanova", "Sergeevna", "Engineer", "anna.ivanova@mail.com", 
        "89536781245", "linkedin.com/in/ivanova", "@anna_iv", {"Moscow", "Tverskaya", 7}},
        {"Dmitry", "Sokolov", "Alexeevich", "Teacher", "dsokolov1985@yandex.ru", 
        "89234567890", "facebook.com/dsokolov", "@sokolov_d", {"Saint-Petersburg", "Nevsky", 28}},
        {"Elena", "Smirnova", "Vladimirovna", "Doctor", "elena_smirnova@rambler.ru", 
        "89015673421", "ok.ru/elenasmirnova", "@smirnova_e", {"Kazan", "Baumana", 11}},
        {"Alexey", "Kuznetsov", "Nikolaevich", "Programmer", "a.kuznetsov@gmail.com", 
        "89123456789", "github.com/kuznetsov", "@kuznetsov_a", {"Yekaterinburg", "Lenina", 45}},
        {"Marina", "Popova", "Fedorovna", "Designer", "marina.popova@mail.ru", 
        "89217654321", "behance.net/popova", "@popova_m", {"Rostov-on-Don", "Gorkogo", 3}},
        {"Sergey", "Orlov", "Evgenievich", "Driver", "sergey.orlov@bk.ru", 
        "89567890123", "vk.com/orlovsergey", "@orlov_s", {"Omsk", "Lenina", 20}},
        {"Natalia", "Vasilieva", "Olegovna", "Lawyer", "n.vasilieva@list.ru", 
        "89876543210", "facebook.com/vasilieva", "@vasilieva_n", {"Samara", "Kuybysheva", 12}},
        {"Victor", "Morozov", "Stepanovich", "Firefighter", "victor.morozov@yandex.ru", 
        "89098765432", "ok.ru/morozov", "@morozov_v", {"Chelyabinsk", "Kirova", 9}},
        {"Olga", "Nikiforova", "Artemovna", "Accountant", "olga.nikiforova@mail.com", 
        "89991234567", "linkedin.com/in/nikiforov", "@nikiforova_o", {"Vladivostok", "Svetlanskaya", 33}}
    };
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
                add_node(&head, id, init_person());
                break;
            case 2:
                printf("Введите id: ");
                scanf("%d", &id);
                if(delete_node(&head, id) == NO_ELEMENT) {printf("Элемент не найден\n"); break;}
                break;
            case 3:
                printf("Введите id: ");
                scanf("%d", &id);
                Node* element = search_node(head, id);
                if (element == NULL) {printf("Нет такого элемента\n"); break;}
                edit_person(get_data(element));
                break;
            case 4:
                print_list(head);
                break;
            case 5:
                printf("Количество тестовых записей: ");
                int n;
                scanf("%d", &n);
                int ids[] = {5, 1, 0, 4, 6, 2, 7, 9, 3, 10};
                for (int i = 0; i < n; i++){
                    add_node(&head, ids[i], &people[i]);
                }
        }
    }
}