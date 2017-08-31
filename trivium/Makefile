include ../config.mk

CC = gcc
CPPFLAGS = -I$(INC)
CFLAGS = -Wall -O3 -std=c99 -g
LDLIBS = -L$(LIB) -lm -ltrivium

TRIVIUM = trivium

.PHONY: all

all: $(TRIVIUM)

%: %.c

clean:
	rm -f *~ $(TRIVIUM)
