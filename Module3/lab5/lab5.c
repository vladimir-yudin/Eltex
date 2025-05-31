#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

static int can_read_file = 0;

void handle_sigusr1(int sig) {
    can_read_file = 0;
}

void handle_sigusr2(int sig) {
    can_read_file = 1;
}

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
    int fd = open("output.txt", O_TRUNC);
    close(fd);
    srand(getpid());
    pid_t pid;
    switch (pid = fork()){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            signal(SIGUSR1, handle_sigusr1);
            signal(SIGUSR2, handle_sigusr2);
            close(pipefd[0]);
            for(int i = 0; i < iterations; i++){
                char buffer[20] = "";
                int input_num = rand();
                sprintf(buffer, "%d\n", input_num);
                if (write(pipefd[1], buffer, strlen(buffer)) == -1){
                    perror("write");
                    exit(1);
                }
                if (can_read_file) {
                    int fd = open("output.txt", O_RDONLY);
                    if (fd == -1){
                        printf("Не удалось открыть файл");
                        exit(1);
                    }
                    if(read(fd, buffer, sizeof(buffer)) == -1){
                        perror("read");
                        exit(1);
                    }
                    if(close(fd) == -1){
                        perror("close");
                    }
                }
                printf("%s", buffer);
            }
            if(close(pipefd[1]) == -1){
                perror("close");
            }
            exit(0);
        default:
            if(close(pipefd[1]) == -1){
                perror("close");
            }
            for(int i = 0; i < iterations; i++){
                char buffer[20] = "";
                if (read(pipefd[0], buffer, 20) == -1){
                    perror("read");
                    return 1;
                }
                if (kill(pid, SIGUSR1) == -1) {
                    perror("kill");
                    return 1;
                }
                    int fd = open("output.txt", O_WRONLY | O_CREAT);
                    if (fd == -1){
                        printf("Не удалось открыть файл");
                        return 1;
                    }
                    lseek(fd, 0L, 2);
                    if (write(fd, buffer, strlen(buffer)) == -1){
                        perror("write");
                        return 1;
                    }
                    if (close(fd) == -1) 
                        perror("close")
                if (kill(pid, SIGUSR2) == -1) {
                    perror("kill");
                    return 1;
                }
            }
            if (close(pipefd[0]) == -1){
                perror("close");
            }
    }
    return 0;
}