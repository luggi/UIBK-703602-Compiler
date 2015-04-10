CFLAGS  = -Wall -Werror -Wno-unused-function -Wno-unused-variable -std=gnu99

.PHONY: all clean

all: main

clean:
	$(RM) *.o main main.c lexer.c tokens.c tokens.h

# main

main: main.o lexer.o list.o tokens.o

main.o: lexer.h tokens.h main.c

main.c: main.c.jin
	python prascal.py < $< > $@

# lexer

lexer.o: lexer.h list.h tokens.h lexer.c

# tokens

tokens.o: tokens.h tokens.c

tokens.c: tokens.c.jin
	python prascal.py < $< > $@

tokens.h: tokens.h.jin
	python prascal.py < $< > $@
