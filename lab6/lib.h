#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

#define QUEUE_NAME "/my_queue"
#define SIZE 256

struct msgbuf {
    long mtype;
    char mtext[SIZE];
};


int msg_snder(int msgid);

int msg_rcver(int msgid);