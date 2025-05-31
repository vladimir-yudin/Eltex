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
    printf("Город: %s\n", p->City);
    printf("Изменить (y/n): ");
    getchar();
    scanf("%c", &o);
    if (o == 'y') {
        char buf[20];
        printf("Новое значение: ");
        scanf("%s", buf);
        strncpy(p->City, buf, 20);
    }
    
    printf("Улица: %s\n", p->Street);
    printf("Изменить (y/n): ");
    getchar();
    scanf("%c", &o);
    if (o == 'y') {
        char buf[20];
        printf("Новое значение: ");
        scanf("%s", buf);
        strncpy(p->Street, buf, 20);
    }

    printf("Дом: %d\n", p->HomeNumber);
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

int write_file(Person* head){
    int fd = open("input.txt", O_RDWR);
    while(head != NULL){
        head = head->next;
        write
    }
}

int read_file(Person* head, char* filename){
    int fd = open("input.txt", O_R);
    char id[10];
    while(read(fd, id, sizeof(id)) > 0){
        Person* n = malloc(sizeof(Person));
        n->id = atoi(id);
        int h = 0;
        for(int i = 0; i < sizeof(i_fields)/sizeof(i_fields[0]); i++){
            char buf[25];
            if(read(fd, buf, i_fields[i]) > 0){
                strncpy(n->FirstName + h, buf, i_fields[i]);
                h += i_fields[i];
            }
        }
        char buf[20];
        if(read(fd, buf, 20) > 0){ strncpy(n->WorkPlace.City, buf, 20); }
        if(read(fd, buf, 20) > 0){ strncpy(n->WorkPlace.Street, buf, 20); }
        if(read(fd, buf, 10) > 0){ strncpy(n->WorkPlace.Street, buf, 10); }

        new_person(head, n);
    }
    close(fd);
}

int main(){
    Person* head = NULL;
    printf("\
1 - Добавление сообщения\n\
2 - Удаление записи\n\
3 - Редактирование записи\n\
4 - Вывод очереди\n\
5 - Вывод в файл\n");
    
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
            default:
                break;
        }
    }
}