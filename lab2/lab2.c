#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

int main(){
    while(1){
        printf("Введите имя программы и её аргументы: \n");
        char buffer[100];
        fgets(buffer, sizeof(buffer), stdin);
        char *istr;
        istr = strtok (buffer," ");
        char* args[10];
        int i = 0;
        while (istr != NULL){
            args[i] = istr;
            if (i >= 10 ){
                printf("Слишком много аргументов\n");
                return 1;
            }
            i++;
            istr = strtok (NULL," \n");
        }
        pid_t pid;
        switch(pid = fork()){
            case -1:
                perror("fork");
                exit(1);
            case 0:
                execv(args[0], args);
                printf("Некорректное завершение программы");
                return 1;
            default:
                wait(NULL);
        }
    }
}