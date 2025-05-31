#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/sem.h>
#include <sys/ipc.h>
#include <string.h>

#define SIZE 32

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

    struct sembuf sem_lock = {0, -1, 0};
    struct sembuf sem_unlock[2] = {{0, 0, 0}, {0, 1, 0}};
    int semid = semget(IPC_PRIVATE, 1, IPC_CREAT | 0600);
    if (semid == -1) { 
        perror("semget"); 
        return 1;
    }
    if (semctl(semid, 0, SETVAL, 1) == -1){
        perror("semctl");
        return 1;
    }
    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }
    int fd = open("output.txt", O_CREAT | O_RDWR | O_TRUNC, 0666);
    if (fd == -1){
        perror("open");
        return 1;
    }
    if (close(fd) == -1) {
        perror("close");
        return 1;
    }
    srand(getpid());
    pid_t pid;
    switch (pid = fork()){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            close(pipefd[0]);
            for(int i = 0; i < iterations; i++){
                char output_buf[SIZE] = "";
                int input_num = rand();
                sprintf(output_buf, "%d\n", input_num);
                if (write(pipefd[1], output_buf, SIZE) == -1){
                    perror("write");
                    exit(1);
                }
                if(semop(semid, &sem_lock, 1) == -1){
                    perror("semop");
                    return 1;
                }
                    char input_buf[SIZE] = "";
                    int fd = open("output.txt", O_RDONLY);
                    if (fd == -1){
                        printf("Не удалось открыть файл");
                        exit(1);
                    }
                    lseek(fd, -1*SIZE, SEEK_END);
                    if(read(fd, input_buf, SIZE) == -1){
                        perror("read");
                        exit(1);
                    }
                    if(close(fd) == -1){
                        perror("close");
                    }
                    printf("%s", input_buf);
                    if(semop(semid, &sem_unlock[0], 1) == -1){
                        perror("semop");
                        return 1;
                    }
                    if(semop(semid, &sem_unlock[1], 1) == -1){
                        perror("semop");
                        return 1;
                    }
            }
            if(close(pipefd[1]) == -1){
                perror("close");
            }
            exit(0);
        default:
            if(close(pipefd[1]) == -1)
                perror("close");
            for(int i = 0; i < iterations; i++){
                char buffer[SIZE] = "";
                if( semop(semid, &sem_lock, 1) == -1){
                    perror("semop");
                    return 1;
                }
                if (read(pipefd[0], buffer, SIZE) == -1){
                    perror("read");
                    return 1;
                }
                    int fd = open("output.txt", O_WRONLY | O_APPEND);
                    if (fd == -1){
                        printf("Не удалось открыть файл 110");
                        return 1;
                    }
                    if (write(fd, buffer, SIZE) == -1){
                        perror("write");
                        return 1;
                    }
                    if (close(fd) == -1) 
                        perror("close");
                if(semop(semid, sem_unlock, 2) == -1){
                    perror("semop");
                    return 1;
                }
            }
            if (close(pipefd[0]) == -1){
                perror("close");
            }
            wait(NULL);
    }
    semctl(semid, 0, IPC_RMID, NULL);
    return 0;
}