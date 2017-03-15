#include "shmfifo.h"
#include <assert.h>

shmfifo_t *shmfifo_init(key_t key, int blksize, int blocks)
{
	shmfifo_t *fifo = (shmfifo_t *)malloc(sizeof(shmfifo_t));
	assert(fifo != NULL);
	memset(fifo, 0, sizeof(shmfifo_t));

	int shmid;
	int size = sizeof(shmhead_t) + blksize*blocks;
	shmid = shmget(key, 0, 0);

	if(shmid == -1)
	{
		fifo->shmid = shmget(key, size, IPC_CREAT | 0666);
		if(fifo->shmid == -1)
			ERR_EXIT("shmfifo_init_shmget");

		fifo->p_shm = (shmhead_t*)shmat(fifo->shmid, NULL, 0);
		if(fifo->p_shm == NULL)
			ERR_EXIT("shmat");
		
		fifo->p_payload = (char*)(fifo->p_shm + 1);

		fifo->p_shm->blksize = blksize;
		fifo->p_shm->blocks = blocks;
		fifo->p_shm->rd_index = 0;		
		fifo->p_shm->wr_index = 0;   

		fifo->sem_mutex = semCreat(key);
		fifo->sem_full = semCreat(key+1);
		fifo->sem_empty = semCreat(key+2);
		
		semSetVal(fifo->sem_mutex, 1);
		semSetVal(fifo->sem_full, blocks);
		semSetVal(fifo->sem_empty, 0);
	}
	else
	{
		fifo->shmid = shmid;
		fifo->p_shm = (shmhead_t*)shmat(fifo->shmid, NULL, 0);
                if(fifo->p_shm == NULL) 
                        ERR_EXIT("shmat");

		fifo->p_payload = (char*)(fifo->p_shm + 1);
		
		fifo->sem_mutex = semOpen(key);
               	fifo->sem_full = semOpen(key+1);
               	fifo->sem_empty = semOpen(key+2);
	}

	return fifo;
}

void shmfifo_put(shmfifo_t *fifo, const void *buf)
{
	sem_p(fifo->sem_full);
	sem_p(fifo->sem_mutex);

	memcpy(fifo->p_payload + fifo->p_shm->blksize * fifo->p_shm->wr_index, buf, fifo->p_shm->blksize);
	fifo->p_shm->wr_index = (fifo->p_shm->wr_index + 1) % (fifo->p_shm->blocks);

	sem_v(fifo->sem_mutex);
	sem_v(fifo->sem_empty);
}

void shmfifo_get(shmfifo_t *fifo, void *buf)
{
        sem_p(fifo->sem_empty);
        sem_p(fifo->sem_mutex);

        memcpy(buf, fifo->p_payload + fifo->p_shm->blksize * fifo->p_shm->rd_index, fifo->p_shm->blksize);
        fifo->p_shm->rd_index = (fifo->p_shm->rd_index + 1) % (fifo->p_shm->blocks);

        sem_v(fifo->sem_mutex);
        sem_v(fifo->sem_full);
}

void shmfifo_destory(shmfifo_t *fifo)
{
	semRm(fifo->sem_mutex);
	semRm(fifo->sem_full);
	semRm(fifo->sem_empty);
	
	shmdt(fifo->p_shm);
	semctl(fifo->shmid, IPC_RMID, 0);

	free(fifo);
}


