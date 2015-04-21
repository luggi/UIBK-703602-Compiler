override CFLAGS  += -Wall -Werror -Wno-unused-function -Wno-unused-variable \
	                -Wno-unused-label -std=gnu99

.PHONY: all clean

all: main

main: main.o stack.o array.o recovery.o lexer.o tokens.o

stack.o: array.o

clean:
	$(RM) *.o main lexer.c
