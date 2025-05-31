#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>
#include <unistd.h> 
#include <signal.h> 

#define SHM_NAME "/my_shm"
#define MAX_NUMBERS 20
static int is_interrupted = 0;

void stop_program(int sigint){
    is_interrupted = 1;
}

struct sembuf chld_lock = {0, -1, 0};
struct sembuf chld_unlock[2] = {{0, 0, 0}, {0, 1, 0}};
struct sembuf par_lock = {1, -1, 0};
struct sembuf par_unlock[2] = {{1, 0, 0}, {1, 1, 0}};
int semid;
int min_max(int n, int* arr){
    if (semop(semid, &chld_lock, 1) == -1){
        perror("semop");
        return -1;
    }
    int min = arr[0];
    int max = arr[0];
    for (int i = 0; i < n; i++){
        min = arr[i] < min ? arr[i] : min;
        max = arr[i] > max ? arr[i] : max;
    }
    arr[n+1] = min;
    arr[n+2] = max;
    if(semop(semid, par_unlock, 2) == -1){
        perror("semop");
        return -1;
    }
    return 0;
}

void iteration(int shmid){
    signal(SIGINT, stop_program);
    int nums = rand() % MAX_NUMBERS + 1;
    if (ftruncate(shmid, (nums + 2)*sizeof(int)) == -1){
        perror("truncate");
        exit(1);
    }
    // Добавляем 2 поля int для добавления минимума и максимума
    int* arr = (int*)mmap(NULL, (nums + 2)*sizeof(int), PROT_WRITE | PROT_READ, MAP_SHARED, shmid, 0);
    if(arr == (void*) -1){
        perror("mmap");
        exit(1);
    }
    for (int i = 0; i < nums; i++){
        arr[i] = rand();
    }
    switch(fork()){
        case -1:
            perror("fork");
            exit(1);
        case 0:
            if (min_max(nums, arr) == -1){
                printf("Ошибка в min_max\n");
                exit(1);
            }
            exit(0);
    }
    if (semop(semid, &par_lock, 1) == -1){
        perror("semop");
        exit(1);
    }
    printf("min: %d, max: %d\n", arr[nums + 1], arr[nums + 2]);
    if (munmap(arr, (nums+2)*sizeof(int)) == -1){
        perror("munmap");
        exit(1);
    }
    if (semop(semid, chld_unlock, 2) == -1){
        perror("semop");
        exit(1);
    }
}

int main(){
    semid = semget(IPC_PRIVATE, 2, IPC_CREAT | 0600);
    if (semid == -1) { 
        perror("semget"); 
        return 1;
    }
    if (semctl(semid, 0, SETVAL, 1) == -1){
        perror("semctl");
        return 1;
    }
    if (semctl(semid, 1, SETVAL, 0) == -1){
        perror("semctl");
        return 1;
    }
    
    int shmid = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0600);
    if(shmid == -1){
        perror("shm_open");
        exit(1);
    }
    srand(getpid());
    int amount = 0;
    while (!is_interrupted){
        iteration(shmid);
        amount ++;
    }
    if (close(shmid) == -1) {
        perror("close");
        exit(1);
    }
    if (shm_unlink(SHM_NAME) == -1) {
        perror("shm_unlink");
        exit(1);
    }
    printf("%d\n", amount);
}