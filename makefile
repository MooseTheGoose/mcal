cfiles := $(wildcard *.c)
hfiles := $(wildcard *.h)

cflags := $(shell pkg-config --cflags glib-2.0)
libs   := $(shell pkg-config --libs glib-2.0)

mcal : $(cfiles) $(hfiles)
	gcc $(cflags) $(libs) $(cfiles) -o $@
