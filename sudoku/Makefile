
CC = gcc
CFLAGS = -g -Wall
srcs = sudoku.c
objs = $(srcs:.c=.o)

exe = bin

.PHONY: bin

bin: $(objs)
	$(CC) $(CFLAGS) -o $@ $<

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@rm -rf $(objs) $(exe)