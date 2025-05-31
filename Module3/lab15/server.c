#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// прототип функции, обслуживающей
// подключившихся пользователей
void dostuff(int);

// Функция обработки ошибок
void error(const char *msg)
{
    perror(msg);
    exit(1);
}

// глобальная переменная – количество
// активных пользователей 
int nclients = 0;

// макрос для печати количества активных
// пользователей 
void printusers()
{ 
	if(nclients)
	{printf("%d user on-line\n",nclients);}
	else {printf("No User on line\n");}
}

int operation(int a, char sign, int b, int *result) {
    switch (sign) {
        case '-': { *result =  a - b; return 0; }
        case '*': { *result = a * b; return 0; }
        case '/': { *result =  b != 0 ? a / b : 0; return -1;}
        default: { *result = a + b; return 0; }
    }
}


int main(int argc, char *argv[])
{
    char buff[1024];    // Буфер для различных нужд
	
	int sockfd, newsockfd; // дескрипторы сокетов
	int portno; // номер порта
	int pid; // id номер потока
    socklen_t clilen; // размер адреса клиента типа socklen_t
    struct sockaddr_in serv_addr, cli_addr; // структура сокета сервера и клиента
	
	// ошибка в случае если мы не указали порт
    if (argc < 2){
        fprintf(stderr,"ERROR, no port provided\n");
        exit(1);
    }
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
    // ошибка при создании сокета
	if (sockfd < 0) 
       error("ERROR opening socket");
	// Шаг 2 - связывание сокета с локальным адресом
    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = atoi(argv[1]);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // сервер принимает подключения на все IP-адреса
    serv_addr.sin_port = htons(portno);
    // вызываем bind для связывания
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) == -1){
        perror("bind");
        exit(1);
    }
    // Шаг 3 - ожидание подключений, размер очереди - 5
    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(serv_addr.sin_addr), ipstr, sizeof(ipstr));
    printf("Удалённый IP: %s, Порт: %d\n", ipstr, ntohs(serv_addr.sin_port));

	listen(sockfd,5);
    clilen = sizeof(cli_addr);
	
	// Шаг 4 - извлекаем сообщение из очереди
	// цикл извлечения запросов на подключение из очереди    
	while (1){
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        if (newsockfd < 0) 
            error("ERROR on accept");
	    nclients++; // увеличиваем счетчик подключившихся клиентов
	    printusers();
        pid = fork();
        if (pid < 0)
            error("ERROR on fork");
        if (pid == 0){
            close(sockfd);
            dostuff(newsockfd);
            exit(0);
        }
        else close(newsockfd);
    }
    close(sockfd);
    return 0; /* we never get here */
}

void dostuff (int sock)
{
	int bytes_recv; // размер принятого сообщения
	int a,b; // переменные для myfunc
    char sig;
	char buff[1024];
	#define str1 "Enter 1 parametr\r\n"
    #define sign "Enter operation sign\r\n"
	#define str2 "Enter 2 parametr\r\n" 
    	
	// отправляем клиенту сообщение
   	write(sock, str1, sizeof(str1));
   
   	// обработка первого параметра
	bytes_recv = read(sock,&buff[0],sizeof(buff));
	if (bytes_recv < 0) error("ERROR reading from socket");
	a = atoi(buff); // преобразование первого параметра в int
	
    write(sock, sign, sizeof(sign));

    bytes_recv = read(sock,&buff[0],sizeof(buff));
	if (bytes_recv < 0) error("ERROR reading from socket");
    sig = buff[0];

	// отправляем клиенту сообщение
	write(sock,str2,sizeof(str2));
	
	bytes_recv = read(sock,&buff[0],sizeof(buff));
	if (bytes_recv < 0) error("ERROR reading from socket");
	b = atoi(buff); // преобразование второго параметра в int
    int result;
    if (operation(a, sig, b, &result) == -1){
        printf("Error in calculation\n");
        nclients--;
        printf("-disconnect\n"); 
	    printusers();
        exit(1);
    }
	snprintf(buff, strlen(buff), "%d", result); // преобразование результата выполнения в строку
	buff[strlen(buff)] = '\n'; // добавление к сообщению символа конца строки
	// отправляем клиенту результат
	write(sock,&buff[0],sizeof(buff));
	
	nclients--; // уменьшаем счетчик активных клиентов
    printf("-disconnect\n"); 
	printusers();
	return;
}