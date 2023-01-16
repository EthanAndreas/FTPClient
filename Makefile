CC = gcc
CFLAGS = -O0 -pipe -Wall -Werror -Wextra -g
LDLIBS ?=  -lreadline

all: client

client: src/client.c
	gcc -o bin/client src/client.c $(CFLAGS) $(LDLIBS)

	@echo "\033[92mCompiled\033[0m"

.PHONY: clean
clean:
	rm -rf build/*.o
	rm -rf bin/*
	rm -rf assets/*
	@echo "\033[92mCleaned\033[0m"