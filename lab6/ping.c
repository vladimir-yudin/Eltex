#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "lib.h"

int main(){
    key_t key = ftok(QUEUE_NAME, 0);
    int msgid = msgget(key, IPC_CREAT);
    while(1){
        if (msg_snder(msgid)) break;
        if (msg_rcver(msgid)) break;
    }
    msgctl(msgid, IPC_RMID, NULL);
}