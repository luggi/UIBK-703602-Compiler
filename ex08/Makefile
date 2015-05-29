override CFLAGS  += -Wall -Werror -Wno-unused-function -Wno-unused-variable \
                    -std=gnu99 -D_POSIX_C_SOURCE=200809L

override YFLAGS  += -d -k

YACC = bison

.PHONY: all clean

all: main

clean:
	$(RM) *.o main lexer.c parser.h parser.c

main: ast.o lexer.o list.o parser.o prascal_writer.o symbol_table.o

main.o: parser.h ast.h

ast.o: parser.h

prascal_writer.o: parser.h

%.c %.h: %.y
	$(YACC) $(YFLAGS) $< -o $(@:.h=.c)
