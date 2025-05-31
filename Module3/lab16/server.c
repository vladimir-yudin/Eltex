#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <sys/ipc.h>
#include <netdb.h> 
#include <arpa/inet.h>

#define SIZE 1024
#define SEM_NAME "/sem_name"
#define SHM_NAME "/shm_name"

enum msg_type{
    FILE_MSG,
    EXPRESSION_MSG,
    TEXT_MSG
} msg_type;

typedef struct message{
    enum msg_type type;
    int current_packet;
    int packet_number;
    char buf[SIZE];
} message;

sem_t* semid;
int shmid;
int* nclients;

int server_func(int sockfd);

int child_func(int newsockfd);

int operation(int a, char sign, int b, int *result) {
    switch (sign) {
        case '-': { *result =  a - b; return 0; }
        case '*': { *result = a * b; return 0; }
        case '/': { 
            if (b == 0) {*result = 0; return -1; }
            *result = a / b; return 0; 
        }
        default: { *result = a + b; return 0; }
    }
}

int main(int argc, char* argv[]){
    if (argc < 2){
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }

    semid = sem_open(SEM_NAME, O_CREAT, 0666, 1);
    if (semid == SEM_FAILED){
        perror("sem_open");
        sem_unlink(SEM_NAME);
        exit(1);
    }
    shmid = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
    if(shmid == -1){
        perror("shm_open");
        exit(1);
    }
    if (ftruncate(shmid, sizeof(int)) == -1){
        perror("truncate");
        exit(1);
    }

    // Добавляем 2 поля int для добавления минимума и максимума
    nclients = (int*) mmap(NULL, sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    if(*nclients ==  -1){
        perror("mmap");
        exit(1);
    }

	int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // ошибка при создании сокета
	if (sockfd == -1){
       perror("socket");
       exit(1);
    }
	// Шаг 2 - связывание сокета с локальным адресом
    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(argv[1]));
    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        perror("bind");
        exit(1);
    }
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serv_addr.sin_addr), ipstr, sizeof(ipstr));
    printf("Удалённый IP: %s, Порт: %d\n", ipstr, ntohs(serv_addr.sin_port));
    
    // Шаг 3 - ожидание подключений, размер очереди - 5
    server_func(sockfd);
       
    if (sem_close(semid) == -1)
        perror("sem_close");
    if (sem_unlink(SEM_NAME) == -1)
        perror("sem_unlink");
    if (munmap(nclients, sizeof(int)) == -1)
        perror("munmap");
    if (close(shmid) == -1) 
        perror("close");
    if (shm_unlink(SHM_NAME) == -1)
        perror("shm_unlink");
    return 0;
}

int server_func(int sockfd){
    listen(sockfd,5);
    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);
    while(1){
        int newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd == -1){
            perror("accept");
            continue;
        }
        if(sem_wait(semid) == -1){
            perror("sem_wait");
            exit(1);
        }
	    *nclients += 1; // увеличиваем счетчик подключившихся клиентов
        if(sem_post(semid) == -1){
            perror("sem_post");
            exit(1);
        }
	    printf("%d user on-line\n",*nclients);
        int pid = fork();
        if (pid == -1)
            perror("fork");
        if (pid == 0){
            close(sockfd);
            child_func(newsockfd);
            if(sem_wait(semid) == -1){
                perror("sem_wait");
                exit(1);
            }
            *nclients -= 1; // уменьшаем счетчик подключившихся клиентов
            if(sem_post(semid) == -1){
                perror("sem_post");
                exit(1);
            }
            printf("%d user on-line\n",*nclients);
            exit(0);
        }
        close(newsockfd);
    }
    return 0;
}

int child_func(int sockfd){
    message msg;
    msg.type = TEXT_MSG;
    msg.current_packet = 1;
    msg.packet_number = 1;
    strcpy(msg.buf, "Connected\r\n");
    if (write(sockfd, &msg, sizeof(msg)) == (ssize_t) -1){
        perror("send");
        return -1;
    }
    if (read(sockfd, &msg, sizeof(msg)) == (ssize_t) -1){
        perror("recv");
        return -1;
    }
    if (msg.type == EXPRESSION_MSG) {
        int a = atoi(msg.buf);
        if (read(sockfd, &msg, sizeof(msg)) == (ssize_t) -1){
            perror("recv");
            return -1;
        }
        char sign = msg.buf[0];
        if (read(sockfd, &msg, sizeof(msg)) == (ssize_t) -1){
            perror("recv");
            return -1;
        }
        int b = atoi(msg.buf);
        int result;
        if (operation(a, sign, b, &result) == -1){
            printf("Bad expression\n");
            return -1;
        }
        printf("%d %c %d = %d\n", a, sign, b, result);
    }
    if (msg_type == FILE_MSG){
        printf("Введите имя файла: ");
        char buf[SIZE];
        scanf("%s", buf);
        int fd = open(buf, O_CREAT | O_WRONLY | O_TRUNC, 0600);
        if (fd == -1){
            perror("open");
            return -1;
        }
        printf("packet: %d/%d : %s\n", msg.current_packet, msg.packet_number, msg.buf);
        if (write(fd, msg.buf, sizeof(msg.buf) - 1) == (ssize_t) -1){
            perror("write");
            return -1;
        }
        for (int i = 1; i < msg.packet_number; i++){
            if (read(sockfd, &msg, sizeof(msg)) == (ssize_t) -1){
                perror("recv");
                return -1;
            }
            printf("packet: %d/%d : %s\n", msg.current_packet, msg.packet_number, msg.buf);
            if (write(fd, msg.buf, sizeof(msg.buf) - 1) == (ssize_t) -1){
                perror("write");
                return -1;
            }
            if (write(sockfd, &msg, sizeof(msg)) == (ssize_t) -1){
                perror("write");
                return -1;
            }
        }
        close(fd);
    }
    return 0;
}