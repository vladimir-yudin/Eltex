#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[]){
    if (argc < 2) return 0;
    double result = 0;
    for(int i = 1; i < argc; i++){
        char* ptrEnd;
        double a =  strtod(argv[i], &ptrEnd);
        if (*ptrEnd != '\0'){
            printf("Некорректно введённые данные\n");
            return 1;
        }
        result = result < a ? a : result;
    }
    printf("%lf\n", result);
}