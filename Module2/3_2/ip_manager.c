#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int check_packets(int* ip_address, int* address_mask, int* packet){
    for(int i = 0; i < 4; i++){
        if ((ip_address[i] & address_mask[i]) != (packet[i] & address_mask[i])) return 0;
    }
    return 1;
}

void read_packet(const char* source, int **a){
    char* src = malloc(strlen(source));
    strcpy(src, source);
    *a = malloc(4 * sizeof(int));
    char *token = strtok(src, ".");
    int counter = 0;
    while(token != NULL){
        char* endptr;
        (*a)[counter] = strtol(token, &endptr, 10);
        if (*endptr != '\0') { free(*a); *a = NULL; break;}
        token = strtok(NULL, ".");
        counter++;
    }
    if (counter < 4) { free(*a); *a = NULL;}
    free(src);
    src = NULL;
}

int main(){
    printf("Введите IP адрес шлюза: \n");
    int* ip_address;
    char buf[15];
    scanf("%s", buf);
    read_packet(buf, &ip_address);
    if(ip_address == NULL) { printf("Неправильно введён ip адрес\n"); return 0; }
    printf("Введите маску подсети: \n");
    int* address_mask;
    scanf("%s", buf);
    read_packet(buf, &address_mask);
    if(address_mask == NULL) { printf("Неправильно введена маска подсети\n"); return 0; }
    printf("Введите количество пакетов: \n");
    int N;
    scanf("%d", &N);
    int counter = 0;
    for (int i = 0; i < N; i++){
        int packet[4];
        for (int j = 0; j < 4; j++){
            packet[j] = rand() % (255 - 0 + 1) + 0;
        }
        counter += check_packets(ip_address, address_mask, packet);
    }
    printf("Подходящих пакетов: %d, %f%%, пакеты для других подсетей: %d", counter, ((float)counter/N) * 100., N - counter);
}