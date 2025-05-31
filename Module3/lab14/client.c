#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 60000
#define MSG_SIZE 1024

int main(int argc, char **argv){
    if(argc != 2){
        printf("Недостаточно аргументов\n");
        exit(1);
        }
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        perror(NULL);
        exit(1);
    }
    struct sockaddr_in cliaddr;
    cliaddr.sin_family = AF_INET;
    cliaddr.sin_port = htons(0);
    cliaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *) &cliaddr, sizeof(cliaddr)) < 0){
        perror(NULL);
        close(sockfd);
        exit(1);
    }
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_aton(argv[1], &servaddr.sin_addr) == 0){
        printf("Неправильный IP address\n");
        close(sockfd);
        exit(1);
    }
    while(1){
        printf("your msg: ");
        char msg_buf[MSG_SIZE];
        fgets(msg_buf, MSG_SIZE, stdin);
        if(sendto(sockfd, msg_buf, strlen(msg_buf) + 1, 0, (struct sockaddr *) &servaddr, sizeof(servaddr)) == -1){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        char rcv_buf[MSG_SIZE];
        if (recvfrom(sockfd, rcv_buf, MSG_SIZE, 0, (struct sockaddr *) NULL, NULL) == -1){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        printf("server: %s", rcv_buf);
    }
    
    close(sockfd);
    return 0;
}