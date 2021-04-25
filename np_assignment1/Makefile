

DBG=-ggdb

SVR=bin/server
CLI=bin/client
TEST=bin/test

BINS=$(wildcard bin/*)


all: libcalc $(TEST) $(CLI) $(SVR)

servermain.o: servermain.cpp
	$(CXX) -Wall $(DBG) -c servermain.cpp -I.

clientmain.o: clientmain.cpp
	$(CXX) -Wall -c clientmain.cpp -I.

main.o: main.cpp
	$(CXX) -Wall -c main.cpp -I.


$(TEST): main.o calcLib.o
	$(CXX) -L./ -Wall -o $@ main.o -lcalc


$(CLI): clientmain.o calcLib.o libtcp
	$(CXX)  -Wall -o $@ clientmain.o -L./ -lcalc -ltcp

$(SVR): servermain.o calcLib.o libtcp
	$(CXX)  -Wall -o $@ servermain.o -L./ -lcalc -ltcp


tcp_server.o: tcp_server.c
	gcc -c   $^ -o $@

tcp_client.o: tcp_client.c
	gcc -c $^ -o $@

log.o: log.c
	gcc -c $^ -o $@

libtcp: tcp_server.o tcp_client.o log.o
	ar -rc libtcp.a  $^



calcLib.o: calcLib.c calcLib.h
	gcc -Wall -fPIC -c calcLib.c

libcalc: calcLib.o
	ar -rc libcalc.a  calcLib.o

clean:
	rm *.o *.a $(BINS)
