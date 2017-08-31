include ../config.mk

# if you wanna see what lies inside the function getEquation during its run,
# just add -DUMP to CFLAGS.
# ...yes, it means I named my macro accordingly
# and no, it doesn't show anything about my political standards
CC = gcc
CPPFLAGS = -I$(INC)
CFLAGS = -Wall -O3 -std=c99 -g
LDLIBS =  -L$(LIB) -ltrivium -lm

EXE = cubatak

.PHONY: all clean force

all: $(EXE)

%: %.c $(INC)/%.h

clean:
	rm -f *~ $(EXE)
