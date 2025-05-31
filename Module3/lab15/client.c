#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 

void error(const char *msg)
{
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[])
{
    int my_sock, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
	char buff[1024];
	
    if (argc < 3) {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }
    // извлечение порта
	portno = atoi(argv[2]);
    
	// Шаг 1 - создание сокета
	my_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (my_sock < 0) {
        perror("ERROR opening socket");
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
        perror("ERROR connecting");
        exit(1);
    }
    
	// Шаг 3 - чтение и передача сообщений
	for (int i = 0; i < 3; i ++){
        recv(my_sock, &buff[0], sizeof(buff) - 1, 0);
        printf("S=>C:%s", buff);

        printf("S<=C:"); 
		fgets(&buff[0], sizeof(buff) - 1, stdin);

        // передаем строку клиента серверу
        send(my_sock, &buff[0], strlen(&buff[0]), 0);
    }
    recv(my_sock, &buff[0], sizeof(buff) - 1, 0);
    printf("S=>C:%s", buff);

    printf("Exit...");
    close(my_sock);
    return 0;
}