# Makefile for xhello application
CFLAGS = -Wall -g
LDFLAGS = -g
LDLIBS = -lX11

APP := xhello

all: $(APP)
$(APP) : $(APP).c

clean:
	rm -f -- $(APP)

.PHONY: all clean
