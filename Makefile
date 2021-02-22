
CPP = g++	
CFLAGS = -std=c++11 -g

#	$(CPP) $(CFLAGS) -o $@ rp_server.o rpcommand.o

OBJECTS = rp_server.o pitaya_interface.o misc.o
#OBJECTS = rp_server.o rpcommand.o pitaya_interface.o
INCLUDES = rsa.h misc.h const_strings.h
SOURCES = rp_server.cpp rp_client.cpp rpcommand.cpp pitaya_interface.cpp


all: rp_server rp_client

misc.o : misc.cpp $(INCLUDES)
	$(CPP) -c $(CFLAGS) $< -o $@

pitaya_interface.o : pitaya_interface.cpp pitaya_interface.h Makefile
	$(CPP) -c $(CFLAGS) $< -o $@

rp_server.o : rp_server.cpp $(INCLUDES) Makefile
	$(CPP) -c $(CFLAGS) $< -o $@
#.cpp.o:
#	$(CPP) -c $(CFLAGS) $< -o $@

rp_server: $(OBJECTS)
	$(CPP) -g $(OBJECTS) -o $@

rp_client: rp_client.cpp $(INCLUDES)
	$(CPP) $(CFLAGS) -o $@ $<
clean:
	rm *.o

