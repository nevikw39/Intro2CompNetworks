CC ?= gcc
CFLAGS += -Iinclude

SRC = $(wildcard src/*.c)
BIN = $(patsubst src/%.c,bin/%,$(SRC))

.DEFAULT_GOAL := all
.PHONY: all clean nevikw39 debug test

debug: CFLAGS += -DDEBUG -g -fdiagnostics-color=always -fsanitize=address -fsanitize=undefined
debug: nevikw39

nevikw39: CFLAGS += -Dnevikw39 -Wall -Wextra
nevikw39: all

all: $(BIN)

bin/client: LOSS_RATE ?= 0.5
bin/client: CFLAGS += -DLOSS_RATE=$(LOSS_RATE)

bin/server: CFLAGS += -pthread
bin/server: LDLIBS += -lm -lpthread

bin/%: src/%.c | bin
	$(CC) $(CFLAGS) $< -o $@ $(LDLIBS)

bin:
	mkdir bin

clean:
	-rm -rf bin/*

test: all
	test/test.sh

doc: doc/README.pdf

doc/README.pdf: doc/README.tex
	latexmk -xelatex -cd doc/README.tex
	latexmk -c -cd doc/README.tex
