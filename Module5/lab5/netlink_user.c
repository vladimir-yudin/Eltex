#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <linux/netlink.h>
#include "constants.h"

int main(int argc, char *argv[])
{
    if (argc < 2){
        printf("Not enough arguments");
        return -1;
    }
    struct sockaddr_nl src_addr, dest_addr;
    struct nlmsghdr *nlh;
    struct iovec iov;
    int sock_fd;
    struct msghdr msg;

    sock_fd = socket(AF_NETLINK, SOCK_RAW, NETLINK_USER);
    if (sock_fd < 0) {
        perror("socket");
        return -1;
    }

    memset(&src_addr, 0, sizeof(src_addr));
    src_addr.nl_family = AF_NETLINK;
    src_addr.nl_pid = getpid();

    if (bind(sock_fd, (struct sockaddr*)&src_addr, sizeof(src_addr)) == -1){
        perror("bind");
        return -1;
    }

    memset(&dest_addr, 0, sizeof(dest_addr));
    dest_addr.nl_family = AF_NETLINK;
    dest_addr.nl_pid = 0;
    dest_addr.nl_groups = 0;

    __u32 length = NLMSG_SPACE(MAX_PAYLOAD);
    nlh = (struct nlmsghdr *)malloc(length);
    memset(nlh, 0, length);
    nlh->nlmsg_len = length;
    nlh->nlmsg_pid = getpid();
    nlh->nlmsg_flags = 0;

    strcpy(NLMSG_DATA(nlh), argv[1]);

    iov.iov_base = (void *)nlh;
    iov.iov_len = nlh->nlmsg_len;
    memset(&msg, 0, sizeof(msg));
    msg.msg_name = (void *)&dest_addr;
    msg.msg_namelen = sizeof(dest_addr);
    msg.msg_iov = &iov;
    msg.msg_iovlen = 1;

    if (sendmsg(sock_fd, &msg, 0) == -1){
        perror("sendmsg");
        close(sock_fd);
        free(nlh);
        return -1;
    }

    if (recvmsg(sock_fd, &msg, 0) == -1){
        perror("recvmsg");
        close(sock_fd);
        free(nlh);
        return -1;
    }
    printf("Received message: %s\n", (char *)NLMSG_DATA(nlh));

    close(sock_fd);
    free(nlh);
    return 0;
}
