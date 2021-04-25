
all: libcalc test client server



servermain.o: servermain.cpp protocol.h 
	$(CXX) -Wall -c servermain.cpp -I.

clientmain.o: clientmain.cpp protocol.h
	$(CXX) -Wall -c clientmain.cpp -I.

main.o: main.cpp protocol.h
	$(CXX) -Wall -c main.cpp -I.


test: main.o calcLib.o
	$(CXX) -L./ -Wall -o test main.o -lcalc

client: clientmain.o calcLib.o libudp 
	$(CXX) -L./ -Wall -o client clientmain.o -lcalc -ludp

server: servermain.o calcLib.o libudp
	$(CXX) -L./ -Wall -o server servermain.o -lcalc -ludp


udp_server.o: udp_server.c common.h
	gcc -Wall -fPIC -c udp_server.c

log.o: log.c common.h
	gcc -Wall -fPIC -c log.c 

libudp: udp_server.o log.o
	ar -rc libudp.a  $^
 
calcLib.o: calcLib.c calcLib.h
	gcc -Wall -fPIC -c calcLib.c

libcalc: calcLib.o
	ar -rc libcalc.a  calcLib.o


clean:
	rm *.o *.a test server client