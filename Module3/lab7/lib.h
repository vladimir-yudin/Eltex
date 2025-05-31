#include <mqueue.h>
#include <stdio.h>
#include <string.h>
#define QUEUE_NAME "/my_queue"
#define SIZE 256

int msg_snder(mqd_t ds);

int msg_rcver(mqd_t ds);