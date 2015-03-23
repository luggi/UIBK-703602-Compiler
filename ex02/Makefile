LEX     = flex
CFLAGS  = -Wall -Werror -Wno-unused-function -std=gnu99
LDFLAGS =
LDLIBS  =

.PHONY: all clean

all: comment_count comment

comment_count: comment_count.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

comment_count.c: comment_count.l
	$(LEX) -o $@ $^

comment: comment.o
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)

comment.c: comment.l
	$(LEX) -o $@ $^


clean:
	$(RM) -f *.o
	$(RM) -f comment_count comment_count.c comment comment.c 
