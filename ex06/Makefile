override CFLAGS  += -Wall -Werror -Wno-unused-function -Wno-unused-variable \
                    -std=gnu99 -D_XOPEN_SOURCE=700

override YFLAGS  += -d -k

YACC = bison

.PHONY: all clean

all: main

clean:
	$(RM) *.o main lexer.c parser.h parser.c

main: lexer.o parser.o ast.o

main.o: parser.h ast.h

asto.o: parser.h

%.c %.h: %.y
	$(YACC) $(YFLAGS) $< -o $(@:.h=.c)
