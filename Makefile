CC = gcc
CFLAGS = -O0 -pipe -Wall -Werror -Wextra -g 
LDLIBS ?=

all: ftp_server ftp_client

ftp_server: src/ftp_server.c
	gcc -o bin/ftp_server src/ftp_server.c

ftp_client: src/ftp_client.c
	gcc -o bin/ftp_client src/ftp_client.c

	@echo "\033[92mCompiled\033[0m"

.PHONY: clean
clean:
	rm -rf build/*.o
	rm -rf bin/*
	@echo "\033[92mCleaned\033[0m"