#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/udp.h>
#include <fcntl.h>

#define BUFFER_SIZE 2048
#define MSG_SIZE 1024

int main() {
    int raw_sock = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    if (raw_sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(raw_sock, (struct sockaddr *)&saddr, sizeof(saddr)) == -1) {
        perror("bind");
        return 1;
    }

    int optval = 1;
    if (setsockopt(raw_sock, IPPROTO_IP, IP_HDRINCL, &optval, sizeof(optval)) < 0) {
        perror("setsockopt");
        return 1;
    }

    int fd = open("write.txt", O_CREAT | O_WRONLY | O_TRUNC, 0600);
    if (fd == -1){
        perror("open");
        return -1;
    }
    char buf[] = "№  SRC_IP_ADRS DST_IP_ADRS PROTO SRC_PRT DST_PRT TEXT\n";
    write(fd, buf, strlen(buf));
    int counter = 0;
    while (1) {
        unsigned char *buffer = (unsigned char *)malloc(BUFFER_SIZE);
        struct sockaddr_in source_addr;
        socklen_t addr_len = sizeof(source_addr);
        ssize_t data_size = recvfrom(raw_sock, buffer, BUFFER_SIZE, 0,
                                     (struct sockaddr *)&source_addr, &addr_len);
        if (data_size == -1) {
            perror("recvfrom");
            break;
        }
        counter++;

        struct iphdr *ip_header = (struct iphdr *)buffer;
        struct in_addr src, dst;
        src.s_addr = ip_header->saddr;
        dst.s_addr = ip_header->daddr;
        char buf[15];
        sprintf(buf, "%d | ", counter);
        write(fd, buf, strlen(buf));
        sprintf(buf, "%s | ", inet_ntoa(src));
        write(fd, buf, strlen(buf));
        sprintf(buf, "%s | ", inet_ntoa(dst));
        write(fd, buf, strlen(buf));
        switch(ip_header->protocol){
            case 6:
                sprintf(buf, "%s | ", "TCP");
                break;
            case 17:
                sprintf(buf, "%s | ", "UDP");
                break;
            default:
                sprintf(buf, "%d | ", ip_header->protocol);
        }
        write(fd, buf, strlen(buf));

        int ip_header_len = ip_header->ihl * 4;

        struct udphdr *udp_header = (struct udphdr *)(buffer + ip_header_len);
        sprintf(buf, "%d | ", ntohs(udp_header->source));
        write(fd, buf, strlen(buf));
        sprintf(buf, "%d | ", ntohs(udp_header->dest));
        write(fd, buf, strlen(buf));
        char msg_buf[MSG_SIZE];
        sprintf(msg_buf, "%s", (char*)buffer + ip_header_len + sizeof(udp_header));
        write(fd, msg_buf, strlen(msg_buf));
        free(buffer);
    }
    close(raw_sock);
    return 0;
}