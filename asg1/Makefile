CC=gcc
CFLAGS= -O3 -Wall -Wextra -pthread
FLAGS= -lcrypt -lpthread
binaries=guessword
.PHONY: clean all

buildrun:
	@$(MAKE) --no-print-directory build
	@$(MAKE) --no-print-directory run

build:
	@$(MAKE) --no-print-directory clean
	@$(MAKE) --no-print-directory guessword

run:
	./guessword

guessword:
	$(CC) $(CFLAGS) -o guessword \guessword.c $(FLAGS)

clean:
	rm -f $(binaries) *.o

fresh:
	rm -f $(binaries) *.o
	clear
