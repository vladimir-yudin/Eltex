#include <stdio.h>
#include "lib.h"

int main(){
    mqd_t ds;
    if ((ds = mq_open(QUEUE_NAME, O_CREAT | O_RDWR)) == (mqd_t)-1){
        perror("Creating queue error");
        return 1;
    }
    while(1){
        if (msg_rcver(ds)) break;
        if (msg_snder(ds)) break;
    }   

    if (mq_close(ds) == -1)
        perror("Closing queue error");
    if (mq_unlink(QUEUE_NAME) == -1)
        perror("Removing queue error");
    return 0;
}