CC=gcc
CFLAGS=-Wall -Wextra -std=c11
BDIR=build

test: $(BDIR)/tester
	$(BDIR)/tester

$(BDIR)/tester: $(BDIR)/7b11b.o $(BDIR)/tester.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm $(BDIR)/*

$(BDIR)/%.o:%.c 7b11b.h
	$(CC) $(CFLAGS) -c -o $@ $<
