
CC = gcc
CFLAGS = -g -Wall
srcs = sudoku.c sudoku_skeleton.c
objs = $(srcs:.c=.o)

exe = bin bin_sk

.PHONY: all
all:bin bin_sk

bin_sk: sudoku_skeleton.o
	@$(CC) $(CFLAGS) -o $@ $<

bin: sudoku.o
	@$(CC) $(CFLAGS) -o $@ $<
    
.c.o:
	@$(CC) $(CFLAGS) -c $< -o $@


.PHONY: clean
clean:
	@rm -rf $(objs) $(exe)