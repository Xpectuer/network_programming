
DBG=-ggdb
CC=gcc
CLI=bin/client 
SVR=bin/server
TST=bin/test
PTHREAD=-pthread
SW=-w


all: directory libcalc $(CLI) $(SVR) $(TST) 	
	echo 'compile complete!'


directory:
	mkdir -p ./bin


servermain.o: servermain.c protocol.h 
	$(CC) -w $(DBG)  -c servermain.c -I.

clientmain.o: clientmain.c protocol.h
	$(CC) -w $(DBG)  -c clientmain.c -I.

main.o: main.cpp protocol.h
	$(CC) -w  -c main.cpp -I.


$(TST): main.o calcLib.o
	$(CC) -L./  -o $@ main.o -ludp

$(CLI): clientmain.o calcLib.o libudp 
	$(CC) -L./  -o $@ clientmain.o  -ludp

$(SVR): servermain.o calcLib.o libudp
	$(CC) -L./  $(PTHREAD)  -o $@ servermain.o  -ludp


udp_server.o: udp_server.c common.h
	$(CC)  -fPIC -c udp_server.c

log.o: log.c common.h
	$(CC)  -fPIC -c log.c 

utils.o: utils.c common.h
	$(CC)  -fPIC -c $<;

accept.o: accept.c common.h
	$(CC)  -fPIC -c $<;


handler.o: handler.c common.h
	$(CC) $(DBG)  -fPIC -c $<;

libudp: udp_server.o log.o utils.o calcLib.o accept.o  handler.o
	ar -rc libudp.a  $^
	rm $^ 
calcLib.o: calcLib.c calcLib.h
	$(CC)  -fPIC -c calcLib.c

libcalc: calcLib.o
	ar -rc libcalc.a  calcLib.o


clean:
	rm *.o *.a $(TST) $(SVR) $(CLI)
