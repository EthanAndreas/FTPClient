CC = gcc
CFLAGS = -O0 -pipe -Wall -Werror -Wextra -g 
LDLIBS ?=

all: server client

server: src/server.c
	gcc -o bin/server src/server.c

client: src/client.c
	gcc -o bin/client src/client.c

	@echo "\033[92mCompiled\033[0m"

.PHONY: clean
clean:
	rm -rf build/*.o
	rm -rf bin/*
	@echo "\033[92mCleaned\033[0m"