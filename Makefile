CONFIG = config.mk

.PHONY: all clean

all: $(CONFIG)

$(CONFIG): init
	./init

clean: $(CONFIG)
	@for dir in */; do       \
	   make -C $$dir clean;  \
	done
	rm -f *~ $(CONFIG)

