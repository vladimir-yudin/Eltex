#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char* argv[]){
    if (argc < 2) {
        printf("Недостаточно аргументов"); 
        return 1;
    }
    char* p_end;
    int iterations = strtol(argv[1], &p_end, 10);
    if (*p_end != '\0') {
        printf("Неправильно введённый аргумент"); 
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    int fd = open("output.txt", O_WRONLY | O_CREAT | O_TRUNC);
    if (fd == -1){
        printf("Не удалось открыть файл");
        return 1;
    }
    srand(getpid());
    pid_t pid = fork();
    switch (pid){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            close(pipefd[0]);
            for(int i = 0; i < iterations; i++){
                int input_num = rand();
                write(pipefd[1], &input_num, sizeof(input_num));
            }
            close(pipefd[1]);
            exit(0);
        default:
            close(pipefd[1]);
            for(int i = 0; i < iterations; i++){
                int output_num;
                read(pipefd[0], &output_num, sizeof(output_num));
                char buffer[20];
                int len = sprintf(buffer, "%d\n", output_num);
                write(fd, buffer, len);
                printf("%d\n", output_num);
            }
            close(pipefd[0]);
            close(fd);
    }
    return 0;
}