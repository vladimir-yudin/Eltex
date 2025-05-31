#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <string.h>

void func(char* argv){
    if (argv == NULL) return;
    char* ptrEnd;
    double a = strtod(argv, &ptrEnd);
    if(*ptrEnd != '\0'){
        printf("Некоректно введённые данные\n");
        return;
    }
    printf("Площадь квадрата со стороной %lf = %lf\n", a, a*a);
    return;
}

int main(int argc, char *argv[]){
    switch(fork()){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            for(int i = 2; i < argc; i += 2){
                func(argv[i]);
            }
            exit(0);
        default:
            for(int i = 1; i < argc; i += 2){
                func(argv[i]);
            }
            wait(NULL);
    }
}