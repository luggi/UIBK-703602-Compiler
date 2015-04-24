override CFLAGS  += -Wall -Werror -Wno-unused-function -Wno-unused-variable \
                    -std=gnu99
override YFLASG  += -d -k

YACC = bison

.PHONY: all clean

all: main

clean:
	$(RM) *.o main lexer.c parser.h parser.c

main: lexer.o parser.o

main.o: parser.h

%.c %.h: %.y
	$(YACC) $(YFLASG) $< -o $(@:.h=.c)
