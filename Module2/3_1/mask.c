#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

void num_to_bit(int value, char **res){
    for (int i = 8; i >= 0; i--){
        sprintf(*res + (8 - i), "%d", (value >> i) & 1);
    }
}

void char_to_bit(char *src, char **res, int start){
    char users_mask[] = "000";
    char rights_mask[] = "000000000";
    int sign_index = -1;
    for (int i = 0; i < strlen(src); i++){
        if (strchr("+-=", src[i]) != NULL){
            sign_index = i; break;
        }
        switch (src[i]){
        case 'u': users_mask[0] = '7'; break;
        case 'g': users_mask[1] = '7'; break;
        case 'o': users_mask[2] = '7'; break;
        case 'a': strcpy(users_mask, "777"); break;
        default:
            free(*res); *res = NULL; return;
        }
    }
    for (int i = sign_index + 1; i < strlen(src); i++){
        switch (src[i]){
        case 'r': rights_mask[0] = '1'; break;
        case 'w': rights_mask[1] = '1'; break;
        case 'x': rights_mask[2] = '1'; break;
        default:
            free(*res); *res = NULL; return;
        }
    }
    memcpy(rights_mask + 3, rights_mask, 3);
    memcpy(rights_mask + 6, rights_mask, 3);
    char *endptr;
    int users_value = strtol(users_mask, &endptr, 8);
    if (*endptr != '\0'){
        free(*res); *res = NULL; return;
    }
    int rights_value = strtol(rights_mask, &endptr, 2);
    if (*endptr != '\0'){
        free(*res); *res = NULL; return;
    }
    switch (src[sign_index]){
    case '+': start |= (users_value & rights_value); break;
    case '-': start &= ~(users_value & rights_value); break;
    case '=': start = (users_value & rights_value); break;
    default:
        free(*res); *res = NULL; return;
    }
    num_to_bit(start, res);
}

void num_to_char(int value, char **res){
    if (*res == NULL) *res == malloc(9);
    for (int i = 2; i >= 0; i--){
        sprintf(*res + 8 - (i * 3 + 2), (value >> (i * 3 + 2)) & 1 ? "r" : "-");
        sprintf(*res + 8 - (i * 3 + 1), (value >> (i * 3 + 1)) & 1 ? "w" : "-");
        sprintf(*res + 8 - (i * 3 + 0), (value >> (i * 3 + 0)) & 1 ? "x" : "-");
    }
}

void num_to_num(int value, char **res){
    if (*res == NULL) *res == malloc(3);
    char str1[3];
    for (int i = 2; i >= 0; i-- ){
        str1[i] = value % 8 + '0';
        value /= 8;
    }
    sprintf(*res, "%s", str1);
}

void command_to_bit(const char *src, char **res, int start){
    if (*res != NULL) free(res);
    *res = malloc(9);

    char *src_copy = malloc(strlen(src));
    strcpy(src_copy, src);
    if (src_copy[0] == 'u' || src_copy[0] == 'g' || src_copy[0] == 'o' || src_copy[0] == 'a'){
        char_to_bit(src_copy, res, start);
    }
    else{
        if (strlen(src_copy) != 3){
            free(*res); *res = NULL; return;
        }
        char *endptr;
        int value = strtol(src, &endptr, 8);
        if (*endptr != '\0'){
            free(*res); *res = NULL; return;
        }
        num_to_bit(value, res);
    }
    free(src_copy);
}

void get_file_rights(struct stat fileStat, char **res) {
    if (*res == NULL) *res = malloc(9);

    sprintf(*res + 0, (fileStat.st_mode & S_IRUSR) ? "1" : "0");
    sprintf(*res + 1, (fileStat.st_mode & S_IWUSR) ? "1" : "0");
    sprintf(*res + 2, (fileStat.st_mode & S_IXUSR) ? "1" : "0");

    sprintf(*res + 3, (fileStat.st_mode & S_IRGRP) ? "1" : "0");
    sprintf(*res + 4, (fileStat.st_mode & S_IWGRP) ? "1" : "0");
    sprintf(*res + 5, (fileStat.st_mode & S_IXGRP) ? "1" : "0");

    sprintf(*res + 6, (fileStat.st_mode & S_IROTH) ? "1" : "0");
    sprintf(*res + 7, (fileStat.st_mode & S_IWOTH) ? "1" : "0");
    sprintf(*res + 8, (fileStat.st_mode & S_IXOTH) ? "1" : "0");
    
}

void print_in_let_nums(const int value, char **result){
    num_to_char(value, result);
    printf("%s\n", *result);
    num_to_num(value, result);
    printf("%s\n", *result);
}

int main(){
    char *result = NULL;
    int value = 0;
    while(1){
        struct stat fileStat;
        char* endptr;
        char command[20];
        printf("\n\
    1 - Перевод в битовое представление\n\
    2 - Вывод прав доступа к файлу\n\
    3 - Изменение прав доступа\n");
        printf("Введите вариант: ");
        int c;
        scanf("%d", &c);
        switch (c){
        case 1:
            printf("Введите права доступа: ");
            scanf("%s", command);

            command_to_bit(command, &result, 0);
            if (result == NULL){
                printf("Неправильно введены права"); break;
            }
            printf("%s\n", result);
            
            value = strtol(result, &endptr, 2);
            break;
        case 2:
            printf("Введите имя файла: ");
            char filename[20];
            scanf("%s", filename);
            if (stat(filename, &fileStat) < 0) {
                printf("Ошибка stat");
                break;
            }
            get_file_rights(fileStat, &result);
            if (result == NULL){
                printf("Неправильно введены права"); break;
            }
            printf("%s\n", result);
            value = strtol(result, &endptr, 2);
            print_in_let_nums(value, &result);
            break;
        case 3:
            printf("Введите команду модификации: ");
            scanf("%s", command);
            command_to_bit(command, &result, value);
            if (result == NULL){
                printf("Неправильно введены права"); break;
            }
            printf("%s\n", result);
            value = strtol(result, &endptr, 2);
            print_in_let_nums(value, &result);
            break;
        default: return 0;
        }
        free(result);
        result = NULL;
    }
    
}