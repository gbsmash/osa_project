CC = gcc
CFLAGS = -Wall

all: server client

server: server.o reader.o writer.o
	$(CC) $(CFLAGS) -o server server.o reader.o writer.o

client: client.o 
	$(CC) $(CFLAGS) -o client client.o

server.o: server.c reader.h writer.h
	$(CC) $(CFLAGS) -c server.c

client.o: client.c
	$(CC) $(CFLAGS) -c client.c

reader.o: reader.c reader.h
	$(CC) $(CFLAGS) -c reader.c

writer.o: writer.c writer.h reader.h
	$(CC) $(CFLAGS) -c writer.c

clean:
	rm -f *.o server client server_pipe
