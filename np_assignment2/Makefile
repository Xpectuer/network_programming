all: libcalc test client server
	clear&&echo 'compile complete!'

DBG=#-ggdb
CC=gcc-10

servermain.o: servermain.c protocol.h 
	$(CC) $(DBG) -Wall -c servermain.c -I.

clientmain.o: clientmain.cpp protocol.h
	$(CC) $(DBG) -Wall -c clientmain.cpp -I.

main.o: main.cpp protocol.h
	$(CC) -Wall -c main.cpp -I.


test: main.o calcLib.o
	$(CC) -L./ -Wall -o test main.o -lcalc

client: clientmain.o calcLib.o libudp 
	$(CC) -L./ -Wall -o client clientmain.o -lcalc -ludp

server: servermain.o calcLib.o libudp
	$(CC) -L./ -Wall -o server servermain.o -lcalc -ludp


udp_server.o: udp_server.c common.h
	$(CC) -Wall -fPIC -c udp_server.c

log.o: log.c common.h
	$(CC) -Wall -fPIC -c log.c 

read.o: read.c common.h
	$(CC) -Wall -fPIC -c read.c

libudp: udp_server.o log.o read.o
	ar -rc libudp.a  $^
 
calcLib.o: calcLib.c calcLib.h
	$(CC) -Wall -fPIC -c calcLib.c

libcalc: calcLib.o
	ar -rc libcalc.a  calcLib.o


clean:
	rm *.o *.a test server client
