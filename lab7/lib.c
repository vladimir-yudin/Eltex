#include "lib.h"

int msg_snder(mqd_t ds){
    printf("Введите приоритет: ");
    int priority;
    scanf("%d", &priority);
    
    printf("Введите сообщение: ");
    char msg_text[SIZE];
    scanf("%s", msg_text);

    if (mq_send(ds, msg_text, strlen(msg_text), priority) == -1){
        perror("Sending message error");
        return 1;
    }
    if (priority == 404) return 1;
    return 0;
}

int msg_rcver(mqd_t ds){
    char msg_text[SIZE];
    int priority;
    if (mq_receive(ds, msg_text, SIZE, &priority) == -1){
        perror("cannot receive");
        return -1;
    }
    printf("%s\n", msg_text);
    if (priority == 404) return 1;
    return 0;
}
