include ../config.mk

# if you wanna see what lies inside the function getEquation during its run,
# just add -DUMP to CFLAGS.
# ...yes, it means I named my macro accordingly
# and no, it doesn't show anything about my political standards
CC = gcc
CPPFLAGS = -I../include -I../trivium
CFLAGS = -Wall -O3 -std=c99 -g
LDLIBS =  -L../trivium -ltrivium -lm

EXE = neutrality

.PHONY: all clean

all: $(EXE)

%: %.c

clean:
	rm -f *~ *.pyc $(EXE)
