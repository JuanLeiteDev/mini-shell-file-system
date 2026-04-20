CC = gcc
SRC = $(wildcard src/commands/*.c)
OBJ = $(patsubst src/commands/%.c,build/obj/%.o,$(SRC))
BIN = $(patsubst build/obj/%.o,build/bin/%,$(OBJ))
UTILS = build/obj/utils.o
INTER = build/obj/interpretador.o

all: build utils interpretador $(OBJ) $(BIN)

build:
	mkdir -p build/bin
	mkdir -p build/obj

interpretador: 
	$(CC) -c src/interpretador.c -o $(INTER)
	$(CC) $(INTER) $(UTILS) -o build/bin/$@

utils:
	$(CC) -c src/include/utils.c -o $(UTILS)

build/obj/%.o: src/commands/%.c
	$(CC) -c $< -o $@

build/bin/%: build/obj/%.o
	$(CC) $< $(UTILS) -o $@

clean:
	rm -rf build