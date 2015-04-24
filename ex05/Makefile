override CFLAGS  += -Wall -Werror -Wno-unused-function -Wno-unused-variable \
                    -std=gnu99
override YFLASG  += -d -k

YACC = bison

.PHONY: all clean

all: main

main: main.o lexer.o parser.h parser.o

main.o: parser.h

clean:
	$(RM) *.o main lexer.c parser.h parser.c

%.c %.h: %.y
	$(YACC) $(YFLASG) $< -o $(@:.h=.c)
