#include "lib.h"

int msg_snder(int msgid){
    struct msgbuf message;
    printf("Введите приоритет:\n");
    scanf("%ld", &message.mtype);
    printf("Введите сообщение:\n");
    scanf("%s", message.mtext);
    if (msgsnd(msgid, &message, sizeof(message.mtext), 0) == -1) {
        perror("msgsnd");
        return 1;
    }
    if (message.mtype == 404) return 1;
    return 0;
}

int msg_rcver(int msgid){
    struct msgbuf message;
    if (msgrcv(msgid, &message, sizeof(message.mtext), 0, 0) == -1) {
        perror("msgrcv");
        return 1;
    }
    printf("%s\n", message.mtext);
    if (message.mtype == 404) return 1;
    return 0;
}
