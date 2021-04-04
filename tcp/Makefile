CC = gcc
OUT = svr
CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o lib/CFILES:.c=.o)
ARCHIVES = $(wildcard lib/*.a)

$(OUT): svr.o lib/libcommon.a
	$(CC) -lm -o $@ $< -L ./lib -l common
	rm $<

lib/read.o: lib/read.c headers
	$(CC) -c -o $@ $<

lib/libcommon.a: lib/read.o
	ar rcs $@ $^
	rm -rf $^

svr.o: tcp_svr.c headers
	$(CC) -c -o $@ $<
 

headers: lib/common.h

lib: lib/libcommon.a

.PHONY: clean

clean:
	rm -rf $(OUT) $(OBJS) $(ARCHIVES)
