#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <netdb.h> 

#define SIZE 1024

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

int main(int argc, char *argv[])
{
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	char buff[SIZE];
	
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    // извлечение порта
	portno = atoi(argv[2]);
    
	// Шаг 1 - создание сокета
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) {
        perror("socket");
        exit(1);
    }
    // извлечение хоста
	server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(1);
    }
    // заполенние структуры serv_addr
	bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
	serv_addr.sin_port = htons(portno);
    
	// Шаг 2 - установка соединения
	if (connect(my_sock,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        perror("connect");
        exit(1);
    }
	// Шаг 3 - чтение и передача сообщений
	message msg;
    if (recv(my_sock, &msg, sizeof(msg)- 1, 0) == (ssize_t) -1){
        perror("recv");
        return -1;
    }
    printf("%s", msg.buf);
    printf("\
    \r1 - Передача выражения\n\
    \r2 - Передача файла\n");
    int oper;
    scanf("%d", &oper);
    getchar();
    switch(oper){
        case 1:
            msg.type = EXPRESSION_MSG;
            msg.packet_number = 3;
            char* chat[3] = {"Введите первое число: ", "Введите знак операции: ", "Введите второе число: "};
            for (int i = 0; i < 3 ; i++){
                msg.current_packet = i + 1;
                printf("%s", chat[i]);
                fgets(msg.buf, SIZE - 1, stdin);
                if (write(my_sock, &msg, sizeof(msg)) == (ssize_t) -1){
                    perror("write");
                    return -1;
                }
            }
            break;
        case 2:
            msg.type = FILE_MSG;
            int fd = open("read.txt", O_RDONLY);
            if (fd == -1){
                perror("open");
                return -1;
            }
            int file_size = (int) lseek(fd, 0, SEEK_END);
            if (file_size == -1) {
                perror("lseek");
                close(fd);
                return 1;
            }
            lseek( fd, 0L, SEEK_SET);
            msg.packet_number = file_size % SIZE == 0 ? file_size / SIZE : (file_size / SIZE + 1);
            for (int i = 0; i < msg.packet_number; i ++){
                msg.current_packet = i + 1;
                bzero(msg.buf, SIZE);
                read(fd, msg.buf, SIZE - 1);
                if (write(my_sock, &msg, sizeof(msg)) == (ssize_t) -1){
                    perror("write");
                    return -1;
                }
            }
            close(fd);
            break;
    }
    printf("Exit...");
    close(my_sock);
    return 0;
}