.PHONY: clean all
cc = gcc
CFLAGS = -Wall -g
BIN = shmfifo_send shmfifo_recv shmfifo_free
OBJS1 = shmfifo_send.o shmfifo.o ipc.o
OBJS2 = shmfifo_recv.o shmfifo.o ipc.o
OBJS3 = shmfifo_free.o shmfifo.o ipc.o
all: $(BIN)
%.o:%.c
	$(cc) $(CFLAGS) -c $< -o $@
shmfifo_send:$(OBJS1)
	$(cc) $(CFLAGS)  $^ -o $@
shmfifo_recv:$(OBJS2)
	$(cc) $(CFLAGS)  $^ -o $@
shmfifo_free:$(OBJS3)
	$(cc) $(CFLAGS)  $^ -o $@
clean:
	rm -f *.o $(BIN)
