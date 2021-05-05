
DBG=-ggdb
CC=gcc
CLI=bin/client 
SVR=bin/server
TST=bin/test
PTHREAD=-pthread



all: directory libcalc $(CLI) $(SVR) $(TST) 	
	echo 'compile complete!'


directory:
	[ ! -d './bin' ] && mkdir  ./bin


servermain.o: servermain.c protocol.h 
	$(CC) $(DBG) -Wall -c servermain.c -I.

clientmain.o: clientmain.c protocol.h
	$(CC) $(DBG) -Wall -c clientmain.c -I.

main.o: main.cpp protocol.h
	$(CC) -Wall -c main.cpp -I.


$(TST): main.o calcLib.o
	$(CC) -L./ -Wall -o $@ main.o -ludp

$(CLI): clientmain.o calcLib.o libudp 
	$(CC) -L./ -Wall -o $@ clientmain.o  -ludp

$(SVR): servermain.o calcLib.o libudp
	$(CC) -L./  $(PTHREAD) -Wall -o $@ servermain.o  -ludp


udp_server.o: udp_server.c common.h
	$(CC) -Wall -fPIC -c udp_server.c

log.o: log.c common.h
	$(CC) -Wall -fPIC -c log.c 


read.o: read.c common.h
	$(CC) -Wall -fPIC -c read.c

utils.o: utils.c common.h
	$(CC) -Wall -fPIC -c $<;

accept.o: accept.c common.h
	$(CC) -Wall -fPIC -c $<;


handler.o: handler.c common.h
	$(CC) $(DBG) -Wall -fPIC -c $<;

libudp: udp_server.o log.o read.o utils.o calcLib.o accept.o  handler.o
	ar -rc libudp.a  $^
	rm $^ 
calcLib.o: calcLib.c calcLib.h
	$(CC) -Wall -fPIC -c calcLib.c

libcalc: calcLib.o
	ar -rc libcalc.a  calcLib.o


clean:
	rm *.o *.a $(TST) $(SVR) $(CLI)
