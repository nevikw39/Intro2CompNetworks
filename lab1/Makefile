CC = gcc
CFLAGS = 
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c,obj/%.o,$(SRC))

.PHONY: all clean nevikw39 debug

all: $(OBJ)
	$(CC) $(CFLAGS) $^

obj/%.o: src/%.c | obj
	$(CC) $(CFLAGS) -c $< -o $@

obj:
	mkdir obj

clean:
	-rm $(OBJ) a.out
	-latexmk -C -cd doc/README.tex

nevikw39: CFLAGS += -Dnevikw39 -Wall -Wextra
nevikw39: all

debug: CFLAGS += -fdiagnostics-color=always -g -DDEBUG -fsanitize=address -fsanitize=undefined
debug: nevikw39

doc: doc/README.pdf

doc/README.pdf: doc/README.tex
	latexmk -xelatex -cd doc/README.tex
	latexmk -c -cd doc/README.tex
