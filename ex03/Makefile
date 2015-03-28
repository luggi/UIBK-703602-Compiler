CFLAGS  = -Wall -Werror -Wno-unused-function -std=gnu99

.PHONY: all clean

all: main

main: main.o lexer.o tokens.o

main.o: tokens.h

main.c: main.c.jin
	python prascal.py < $< > $@

tokens.c: tokens.c.jin
	python prascal.py < $< > $@

tokens.h: tokens.h.jin
	python prascal.py < $< > $@

clean:
	$(RM) *.o main main.c lexer.c tokens.c tokens.h
