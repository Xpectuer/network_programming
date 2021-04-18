CC = gcc
SVR = ./bin/svr
CLI = ./bin/cli
CFILES = $(wildcard *.c)
OBJS = $(CFILES:.c=.o lib/CFILES:.c=.o)
ARCHIVES = $(wildcard lib/*.a)
BINS = $(wildcard bin/*)

all: $(CLI) $(SVR)

$(CLI): cli.o lib/libcommon.a
	$(CC) -lm -o $@ $< -L ./lib -l common
	rm $<


$(SVR): svr.o lib/libcommon.a
	$(CC) -lm -o $@ $< -L ./lib -l common
	rm $<

lib/read.o: lib/read.c headers
	$(CC) -c -o $@ $<

lib/log.o: lib/log.c headers
	$(CC) -c -o $@ $<

lib/tcp_client.o: lib/tcp_client.c headers
	$(CC) -c -o $@ $<

lib/tcp_server.o: lib/tcp_server.c headers
	$(CC) -c -o $@ $<
lib/libcommon.a: lib/read.o lib/log.o lib/tcp_client.o lib/tcp_server.o
	ar rcs $@ $^
	rm -rf $^

svr.o: tcp_svr.c headers
	$(CC) -c -o $@ $<

cli.o: tcp_cli.c headers
	$(CC) -c -o $@ $<


headers: lib/common.h

lib: lib/libcommon.a

.PHONY: clean

clean:
	rm -rf $(OUT) $(OBJS) $(ARCHIVES) $(BINS)
