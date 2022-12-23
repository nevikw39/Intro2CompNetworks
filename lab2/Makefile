CC ?= gcc
CFLAGS ?=

.PHONY: all clean nevikw39 debug

debug: CFLAGS += -DDEBUG -g -fdiagnostics-color=always -fsanitize=address -fsanitize=undefined
debug: nevikw39

nevikw39: CFLAGS += -Dnevikw39 -Wall -Wextra
nevikw39: all

all: client server

client: LOSS_RATE ?= 0.5
client: CFLAGS += -DLOSS_RATE=$(LOSS_RATE)
client: client.c
	$(CC) $(CFLAGS) client.c -o client

server: server.c
	$(CC) $(CFLAGS) -lm -lpthread server.c -o server

clean:
	-rm -f client server
