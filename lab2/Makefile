CC ?= gcc
CFLAGS ?=
LDFLAGS ?=

SRC = $(wildcard src/*.c)
BIN = $(patsubst src/%.c,bin/%,$(SRC))

.PHONY: all clean nevikw39 debug test

debug: CFLAGS += -DDEBUG -g -fdiagnostics-color=always -fsanitize=address -fsanitize=undefined
debug: nevikw39

nevikw39: CFLAGS += -Dnevikw39 -Wall -Wextra
nevikw39: all

all: $(BIN)

bin/client: LOSS_RATE ?= 0.5
bin/client: CFLAGS += -DLOSS_RATE=$(LOSS_RATE)

bin/server: LDFLAGS += -lm -lpthread

bin/%: src/%.c | bin
	$(CC) $(CFLAGS) $(LDFLAGS) $< -o $@

bin:
	mkdir bin

clean:
	-rm -rf bin/*

test: all
	test/test.sh
