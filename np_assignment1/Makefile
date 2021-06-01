DBG=-ggdb

SVR=bin/server
CLI=bin/client
TEST=bin/test


BINS=$(wildcard bin/*)
CC=gcc

all: directory libcalc $(TEST) $(CLI) $(SVR)
	echo 'compile complete!'
	
directory: 
	mkdir -p ./bin


servermain.o: servermain.c
	$(CC) -Wall $(DBG) -c servermain.c -I.

clientmain.o: clientmain.c
	$(CC) -Wall -c clientmain.c -I.

main.o: main.cpp
	$(CC) -Wall -c main.cpp -I.


$(TEST): main.o calcLib.o
	$(CC) -L./ -Wall -o $@ main.o -lcalc


$(CLI): clientmain.o calcLib.o libtcp
	$(CC)  -Wall -o $@ clientmain.o -L./ -lcalc -ltcp

$(SVR): servermain.o calcLib.o libtcp
	$(CC)  -Wall -o $@ servermain.o -L./ -lcalc -ltcp


tcp_server.o: tcp_server.c
	gcc -c  $^ -o $@

tcp_client.o: tcp_client.c
	gcc -c $^ -o $@

log.o: log.c
	gcc -c $^ -o $@

handlers.o: handlers.c
	$(CC) -c $^ -o $@

libtcp: tcp_server.o tcp_client.o log.o handlers.o
	ar -rc libtcp.a  $^



calcLib.o: calcLib.c calcLib.h
	gcc -Wall -fPIC -c calcLib.c

libcalc: calcLib.o
	ar -rc libcalc.a  calcLib.o

clean:
	rm *.o *.a $(BINS)
