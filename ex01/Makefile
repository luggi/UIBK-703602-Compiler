LEX     = flex
CFLAGS  = -Wall -Werror -Wno-unused-function
LDFLAGS =
LDLIBS  = -lfl

.PHONY: clean

new_sample: new_sample.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

new_sample.o: new_sample.c
	$(CC) $(CFLAGS) -c -o $@ $^

new_sample.c: new_sample.l
	$(LEX) -o $@ $^

clean:
	$(RM) -f *.o
	$(RM) -f new_sample new_sample.c
