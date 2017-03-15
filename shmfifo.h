#ifndef _SHM_FIFO_H
#define _SHM_FIFO_H

#include "ipc.h"

typedef struct shmhead shmhead_t;
typedef struct shmfifo shmfifo_t;

struct shmhead
{
	unsigned int blksize;
	unsigned int blocks;
	unsigned int rd_index;
	unsigned int wr_index;
};

struct shmfifo
{
	shmhead_t *p_shm;
	char *p_payload;
	
	int shmid;
	int sem_mutex;
	int sem_full;
	int sem_empty;
};

shmfifo_t *shmfifo_init(key_t key, int blksize, int blocks);
void shmfifo_put(shmfifo_t *fifo, const void *buf);
void shmfifo_get(shmfifo_t *fifo, void *buf);
void shmfifo_destory(shmfifo_t *fifo);

#endif
