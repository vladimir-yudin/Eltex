#include <stdio.h>
#include <string.h>

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

typedef struct Person {
    int id;
    char FirstName[20], Surname[20], LastName[20], JobTitle[20], Email[20];
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

void new_person_lite(Person *p, int id){
    p->id = id;
    int h = 0;
    for (int i = 0; i < 2; i++){
        printf("%s: ", c_field[i]);
        char buf[25];
        scanf("%s", buf);
        strncpy(p->FirstName + h, buf, i_fields[i]);
        h += i_fields[i];
    }
}

void new_person(Person *p, int id){
    p->id = id;
    static int h = 0;
    for (int i = 0; i < sizeof(c_field)/sizeof(c_field[0]); i++){
        printf("%s: ", c_field[i]);
        char buf[25];
        scanf("%s", buf);
        strncpy(p->FirstName + h, buf, i_fields[i]);
        h += i_fields[i];
    }
    add_Place(&p->WorkPlace);
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

int check_person(char* name, char* surname, Person *p){
    if (strcmp(name, p->FirstName) == 0 && strcmp(surname, p->Surname) == 0) return 1;
    return 0;
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

int search_person(char* name, char* surname, Person* list, int size){
    int result = -1;
    for (int i = 0; i < size; i++){
        if (check_person(name, surname, &list[i])){
            print_person(&list[i]);
            result = i;
        };
    }
    return result;
}

int main() {
    Person p[10];
    int n = 10; //макс количество записей в массиве
    int k = 0; //текущее количество записей в массиве
    while(1){
        
        printf("\
        1 - Добавление человека (только имя и фамилия)\n\
        2 - Добавление человека (полный ввод данных)\n\
        3 - Редактирование записи\n\
        4 - Удаление записи\n\
        5 - Просмотр всех контактов\n");
        int c;
        printf("Введите вариант: ");
        scanf("%d", &c);
        switch(c){
            case 1:
                new_person_lite(&p[k], k);
                k += 1;
                break;
            case 2:
                new_person(&p[k], k);
                k += 1;
                break;
            case 3:
                if (k == 0) {printf("Нет записей\n"); break;}
                printf("Введите имя человека: ");
                char name_buf[20];
                scanf("%s", name_buf);
                printf("Введите фамилию человека: ");
                char surname_buf[20];
                scanf("%s", surname_buf);

                int index = search_person(name_buf, surname_buf, p, k);
                if (index < 0) {printf("Нет подходящего человека\n"); break;}

                printf("Введите id человека: ");
                scanf("%d", &index);
                if (!check_person(name_buf, surname_buf, &p[index])){ printf("Нет такой записи\n"); break;}
                edit_person(&p[index]);
                break;
            case 4:
                if (k == 0) {printf("Нет элементов для удаления\n"); break;}
                printf("Введите имя человека: ");
                name_buf[20];
                scanf("%s", name_buf);
                printf("Введите фамилию человека: ");
                surname_buf[20];
                scanf("%s", surname_buf);

                index = search_person(name_buf, surname_buf, p, k);
                if (index < 0) {printf("Нет подходящего человека\n"); break;}

                printf("Введите id человека: ");
                scanf("%d", &index);
                if (!check_person(name_buf, surname_buf, &p[index])){ printf("Нет такой записи\n"); break;}
                
                memcpy(&p[index], &p[k-1], sizeof(Person));
                p[index].id = index;
                k -= 1;
                break;
            case 5:
                if (k == 0) {printf("Нет элементов для вывода\n"); break;}
                for (int i = 0; i < k; i++){
                    print_person(&p[i]);
                }
                break;
            default:
                break;;
        }
    }
    return 0;
}