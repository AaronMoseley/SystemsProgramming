CFLAGS = -Wall
HFILES = proj5.h

all: parent child

parent: $(HFILES) await.o parent.o
	gcc $(CFLAGS) -o parent await.o parent.o
child:	$(HFILES) await.o child.o
	gcc $(CFLAGS) -o child await.o child.o
