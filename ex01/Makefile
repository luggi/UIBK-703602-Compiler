LEX     = flex
CFLAGS  = -Wall -Werror -Wno-unused-function -std=gnu99
LDFLAGS =
LDLIBS  =

.PHONY: all clean

all: new_sample simple_example

new_sample: new_sample.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

new_sample.c: new_sample.l
	$(LEX) -o $@ $^

simple_example: simple_example.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

simple_example.c: simple_example.l
	$(LEX) -o $@ $^

clean:
	$(RM) -f *.o
	$(RM) -f new_sample new_sample.c
	$(RM) -f simple_example simple_example.c
