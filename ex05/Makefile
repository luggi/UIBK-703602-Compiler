
.PHONY: all clean

all: main

main: main.o parser.o lexer.o

parser.c: parser.y
	bison -d $< -o $@

clean:
	$(RM) *.o main lexer.c parser.c parser.h
