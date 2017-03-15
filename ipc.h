#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<string.h>

#include<unistd.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/sem.h>
#include<sys/shm.h>


#define ERR_EXIT(m)\
        do\
        {\
                perror(m);\
                exit(EXIT_FAILURE);\
        } while(0)


int semCreat(key_t key);
int semOpen(key_t key);
int semRm(int semid);
int semSetVal(int semid, int val);
int semGetVal(int semid);
int sem_p(int semid);
int sem_v(int semid);





