CC = gcc
CFLAGS = -std=c11

.PHONY: all clean nevikw39 debug

debug: CFLAGS += -fdiagnostics-color=always -g -DDEBUG -fsanitize=address -fsanitize=undefined
debug: nevikw39

nevikw39: CFLAGS += -Dnevikw39 -Wall -Wextra
nevikw39: all

all: client server

client: client.c
	$(CC) $(CFLAGS) client.c -o client

server: server.c
	$(CC) $(CFLAGS) -lm -lpthread server.c -o server

clean:
	-rm -f client server
