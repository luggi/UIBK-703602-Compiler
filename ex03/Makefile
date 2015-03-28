CFLAGS  = -Wall -Werror -Wno-unused-function -std=gnu99

.PHONY: all clean

all: main

main: main.o lexer.o tokens.o

main.c: main.c.in
	python prascal.py

tokens.c: tokens.c.in
	python prascal.py

tokens.h: tokens.h.in
	python prascal.py

clean:
	$(RM) *.o main main.c lexer.c tokens.c tokens.h
