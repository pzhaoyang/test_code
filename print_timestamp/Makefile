bin := timestamp
hdr := timestamp.h
src := main.c
CC := gcc
CFLGS = -g -W
RM = rm -rf

.PHONY: all clean
all:$(bin)

${bin}: $(src)
	$(CC) -o $@ $< $(CFLGS)

clean:
	$(RM) $(bin)