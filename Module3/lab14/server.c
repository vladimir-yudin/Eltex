#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 60000
#define MSG_SIZE 1024

int main() {
    int sockfd;
    if ((sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1){
        perror("socket");
        exit(1);
    }
    struct sockaddr_in servaddr;
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(sockfd, (struct sockaddr *) &servaddr, sizeof(servaddr)) < 0){
        perror("bind");
        close(sockfd);
        exit(1);
    }

    char ipstr[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &(servaddr.sin_addr), ipstr, sizeof(ipstr));
    printf("Удалённый IP: %s, Порт: %d\n", ipstr, ntohs(servaddr.sin_port));
    
    while(1){
        struct sockaddr cliaddr;
        int clilen = sizeof(cliaddr);
        char rcv_buf[MSG_SIZE];
        if(recvfrom(sockfd, rcv_buf, MSG_SIZE, 0, (struct sockaddr *) &cliaddr, &clilen) == -1){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
        printf("client: %s", rcv_buf);
        printf("your msg: ");
        char msg_buf[MSG_SIZE];
        fgets(msg_buf, MSG_SIZE, stdin);
        if(sendto(sockfd, msg_buf, strlen(msg_buf) + 1, 0, (struct sockaddr *) &cliaddr, clilen) == -1){
            perror(NULL);
            close(sockfd);
            exit(1);
        }
    }
    close(sockfd);
    return 0;
}