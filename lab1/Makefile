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
	rm $(OBJ) a.out

nevikw39: CFLAGS += -Dnevikw39 -Wall -Wextra
nevikw39: all

debug: CFLAGS += -fdiagnostics-color=always -g -DDEBUG -fsanitize=address -fsanitize=undefined
debug: nevikw39
