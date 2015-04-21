override CFLAGS  += -Wall -Werror -Wno-unused-function -Wno-unused-variable -std=gnu99

.PHONY: all clean

all: main

main: main.o recovery.o lexer.o tokens.o

clean:
	$(RM) *.o main lexer.c
