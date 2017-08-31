include ../config.mk

CC = gcc
CFLAGS = -g -std=c99 -Wall -O3 $(MODE)
CPPFLAGS = -I$(INC)

EXE = all_subsets random_stream

.PHONY: all clean

all: $(EXE)

%: %.c

clean:
	rm -f *~ $(EXE)

