#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <string.h>

#define SEM_PARENT "/sem_parent"
#define SEM_CHILD "/sem_child"
#define NUM_READERS 5
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

    sem_t* sem_parent = sem_open(SEM_PARENT, O_CREAT, 0666, NUM_READERS);
    sem_t* sem_child = sem_open(SEM_CHILD, O_CREAT, 0666, 0);
    if (sem_parent == SEM_FAILED || sem_child == SEM_FAILED){
        perror("sem_open");
        sem_unlink(SEM_PARENT);
        sem_unlink(SEM_CHILD);
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
            if(close(pipefd[0]) == -1)
                perror("close");
            for(int i = 0; i < iterations; i++){
                char output_buf[SIZE] = "";
                int input_num = rand();
                sprintf(output_buf, "%d\n", input_num);
                if (write(pipefd[1], output_buf, SIZE) == -1){
                    perror("write");
                    exit(1);
                }
            }
            if(close(pipefd[1]) == -1){
                perror("close");
            }
            exit(0);
    }
    for(int i = 0; i < iterations; i++){
        for(int j = 0; j < NUM_READERS; j++){
            switch(fork()){
                case -1:
                    perror("fork");
                    exit(1);
                case 0:
                    if(sem_wait(sem_child) == -1){
                        perror("sem_wait");
                        exit(1);
                    }
                        char input_buf[SIZE] = "";
                        int fd = open("output.txt", O_RDONLY);
                        if (fd == -1){
                            perror("open");
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
                    if(sem_post(sem_parent) == -1){
                        perror("sem_post");
                        exit(1);
                    }
                    exit(0);
            }
        }
    }
    if(close(pipefd[1]) == -1)
        perror("close");
    for(int i = 0; i < iterations; i++){
        for(int j = 0; j < NUM_READERS; j ++){
            if(sem_wait(sem_parent) == -1){
                perror("sem_wait");
                return 1;
            }
        }
            char buffer[SIZE] = "";
            if (read(pipefd[0], buffer, SIZE) == -1){
                perror("read");
                return 1;
            }
            int fd = open("output.txt", O_WRONLY | O_APPEND);
            if (fd == -1){
                perror("open");
                return 1;
            }
            if (write(fd, buffer, SIZE) == -1){
                perror("write");
                return 1;
            }
            if (close(fd) == -1) 
                perror("close");
        for (int j = 0; j < NUM_READERS; j++){
            if(sem_post(sem_child) == -1){
                perror("sem_post");
                return 1;
            }
        }
    }
    if (close(pipefd[0]) == -1){
        perror("close");
    }
    for(int j = 0; j < NUM_READERS; j ++){
        if(sem_wait(sem_parent) == -1){
            perror("sem_wait");
            return 1;
        }
    }
    sem_close(sem_parent);    
    sem_close(sem_child);
    sem_unlink(SEM_PARENT);
    sem_unlink(SEM_CHILD);
    return 0;
}