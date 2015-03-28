CFLAGS  = -Wall -Werror -Wno-unused-function -std=gnu99

.PHONY: all clean

all: main

main: main.o lexer.o tokens.o

clean:
	$(RM) *.o main lexer.c
