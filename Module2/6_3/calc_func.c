#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void add(double a, double b, double** result){**result = (a + b);}

void substract(double a, double b, double** result){**result = a - b;}

void multiply(double a, double b, double** result){**result = a * b;}

void division(double a, double b, double** result){
    if (b == 0) { free(*result); *result = NULL;}
    else **result = a / b;
}

void (*operation(char sign))(double, double, double**) {
    switch (sign) {
        case '-': return substract;
        case '*': return multiply;
        case '/': return division;
        default: return add;
    }
}
