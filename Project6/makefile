CFLAGS = -Wall
LFLAGS = -lpthread

all:	seqdriver pooldriver

seqdriver:	pq.h seqdriver.c pqimpl.o
	gcc $(CFLAGS) -o seqdriver seqdriver.c pqimpl.o $(LFLAGS)

pooldriver:	threadpool.h pooldriver.c threadpool.o pqimpl.o
	gcc $(CFLAGS) -o pooldriver pooldriver.c threadpool.o pqimpl.o $(LFLAGS)

pqimpl.o:   pq.h pqimpl.c
	gcc $(CFLAGS) -c pqimpl.c

threadpool.o:	threadpool.h threadpool.c
	gcc $(CFLAGS) -c threadpool.c
