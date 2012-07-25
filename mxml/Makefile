.PHONY: clean

all: libmxml.a

libmxml.a: mxml-attr.o mxml-entity.o mxml-file.o mxml-get.o \
		   mxml-index.o mxml-node.o mxml-private.o mxml-search.o \
		   mxml-set.o mxml-string.o
	rm -f $@
	ar rcs $@ $^

%.o: %.c config.h mxml.h mxml-private.h
	gcc -c -o $@ $< -Os

clean:
	rm -f *.o libmxml.a
