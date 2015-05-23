override CFLAGS  += -Wall -Werror -Wno-unused-function -Wno-unused-variable \
                    -std=gnu99

override YFLAGS  += -d -k

YACC = bison

.PHONY: all clean

all: main

clean:
	$(RM) *.o main lexer.c parser.h parser.c

main: graphviz_writer.o prascal_writer.o lexer.o parser.o ast.o

main.o: parser.h ast.h

ast.o: parser.h

prascal_writer.o: parser.h

graphviz_writer.o: parser.h

%.c %.h: %.y
	$(YACC) $(YFLAGS) $< -o $(@:.h=.c)
