
CPP = g++	
CFLAGS = -std=c++11 -g

# rp_server: rp_server.o rpcommand.o Makefile rsa_types.h comm.h
#	$(CPP) $(CFLAGS) -o $@ rp_server.o rpcommand.o

OBJECTS = rp_server.o rpcommand.o
INCLUDES = rsa_types.h comm.h
SOURCES = rp_server.cpp rp_client.cpp rpcommand.cpp


all: rp_server rp_client

.cpp.o:
	$(CPP) -c $(CFLAGS) $< -o $@

rp_server: $(OBJECTS)
	$(CPP) -g $(OBJECTS) -o $@

rp_client: rp_client.cpp $(INCLUDES)
	$(CPP) $(CFLAGS) -o $@ $<
clean:
	rm *.o

