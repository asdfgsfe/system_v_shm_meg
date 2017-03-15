#include "shmfifo.h"


union semun 
{
	int              val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                           (Linux-specific) */
};


int semCreat(key_t key)
{
	int semid;
	semid = semget(key, 1, IPC_CREAT | IPC_EXCL | 0666);
	if(semid < 0)
		ERR_EXIT("semid_creat");

	return semid;
}

int semOpen(key_t key)
{
	int semid;
	semid = semget(key, 0, 0);
	if(semid < 0)
		ERR_EXIT("semOpen_ipc_c");		

	return semid;
}

int semRm(int semid)
{
	int ret = semctl(semid, 0, IPC_RMID, 0);
	if(ret < 0)
		ERR_EXIT("semRm_ipc_c");
	
	return 0;
}

int semSetVal(int semid, int val)
{
	union semun su;
	su.val = val;

	int ret = semctl(semid, 0, SETVAL, su);
	if(ret < 0)
		ERR_EXIT("semsetVal");
	
	return 0;
}

int semGetVal(int semid)
{
        int ret = semctl(semid, 0, GETVAL, 0);
        if(ret < 0)
                ERR_EXIT("semGetVal");

        return ret;
}

int sem_p(int semid)
{
	struct sembuf sb_p = {0, -1, 0};
	int ret;
	ret = semop(semid, &sb_p, 1);
	if(ret < 0)
		ERR_EXIT("sem_p");

	return ret;
}
	
int sem_v(int semid)
{
        struct sembuf sb_v = {0, 1, 0};
        int ret;
        ret = semop(semid, &sb_v, 1);
        if(ret < 0)
                ERR_EXIT("sem_v");

        return ret;
}

