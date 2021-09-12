CC=gcc
CFLAGS= -O3 -Wall -Wextra -pthread
FLAGS= -lcrypt -lpthread
binaries=guessword
.PHONY: clean all

build:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory guessword


guessword:
	$(CC) $(CFLAGS) -o guessword \guessword.c $(FLAGS)

clean:
	rm -f $(binaries) *.o

fresh:
	rm -f $(binaries) *.o
	clear
