#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void add(double a, double b, double** result){**result = a + b;}

void substract(double a, double b, double** result){**result = a - b;}

void multiply(double a, double b, double** result){**result = a * b;}

void division(double a, double b, double** result){
    if (b == 0) { free(*result); *result = NULL;}
    else **result = a / b;
    
}

void (*operation(char sign))(double, double, double** result){
    switch(sign){
        case '-': return substract;
        case '*': return multiply;
        case '/': return division;
        default: return add;
    }
}

void get_sign_index(char* signs, char* required, int** result){
    **result = strlen(signs);
    for(int i = 0; i < strlen(required); i++) {
        char* sign_loc = strchr(signs, required[i]);
        if (sign_loc == NULL) continue;
        int index = sign_loc - signs;
        if (**result > index) **result = index;
    }
}

void ordinal_calculation(double* numbers, char* signs, int counter, char* required, double** result){
    int* i = malloc(sizeof(int));
    get_sign_index(signs, required, &i);
    while (*i != strlen(signs)){
        void (*func)(double, double, double**);
        func = operation(signs[*i]);
        func(numbers[*i], numbers[*i + 1], result);
        if (*result == NULL) break;
        numbers[*i+1] = **result;
        memmove(&numbers[*i], &numbers[*i+1], (counter - *i - 1) * sizeof(double));
        memmove(&signs[*i], &signs[*i+1], (counter - *i - 1) * sizeof(char));
        counter -= 1;
        
        get_sign_index(signs, required, &i);
    }
    free(i);
}

void count(double* numbers, char* signs, int counter, double** result){
    
    double* numbers_copy = malloc(counter * sizeof(double));
    memcpy(numbers_copy, numbers, counter * sizeof(double));
    char* signs_copy = malloc(sizeof(signs));
    strcpy(signs_copy, signs);
    do
    {
        ordinal_calculation(numbers_copy, signs_copy, counter, "*/", result);
        if (*result == NULL) break;
        ordinal_calculation(numbers_copy, signs_copy, counter, "+-", result);
        if (*result == NULL) break;
    } while (0);
    free(numbers_copy);
    free(signs_copy);
}

int read_expression(char* expression, double* numbers, char* signs){
    char* exp_copy = malloc(strlen(expression));
    strcpy(exp_copy, expression);
    char marks[] = "+-*/"; //символы которые разделяют наши числа
    char *token = strtok(exp_copy, marks); //подстрока в которой мы храним число
    int counter = 0; //счётчик количества чисел в выражениии
    int position = 0; //счётчик позиции при чтении выражения
    while (token != NULL){
        char* endptr;
        numbers[counter] = strtod(token, &endptr);
        if (*endptr != '\0') return 0;
        position += strlen(token) + 1;
        signs[counter] = expression[position - 1];
        counter += 1;
        token = strtok(NULL, marks);
    }
    free(exp_copy);
    return counter;
}

int main(){
    printf("Введите выражение: ");
    char expression[50];
    scanf("%s", expression);
    double* numbers = malloc(10*sizeof(double));
    char* signs = malloc(9*sizeof(double));
    int counter = read_expression(expression, numbers, signs);
    if (counter < 2 || strlen(signs) != counter - 1 ) { printf("Ошибка в записи выражения\n"); return 0;}
    
    double* result = malloc(sizeof(double));
    count(numbers, signs, counter, &result);

    if (result == NULL) printf("Ошибка в выражении\n");
    else {printf("Результат: %f\n", *result); free(result);}
}