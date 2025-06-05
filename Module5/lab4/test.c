#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "constants.h"

int main(int argc, char *argv[])
{
    if (argc < 2)
        return -1
    char buf[BUF_LEN];
    char i = 0;
    memset(buf, 0, BUF_LEN);
    printf("Input: %s\n", argv[1]);
    char path[100];
    snprintf(path, sizeof(path), "/dev/%s", DEVICE_NAME);

    int fp = open(path, O_RDWR);
    write(fp, argv[1], strlen(argv[1]));
    read(fp, &buf, BUF_LEN);
    close(fp);
    printf("Output: %s\n", buf);
    return 0;
}