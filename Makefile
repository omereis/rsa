#*****************************************************************************\
#                                 Makefile                                    |
#*****************************************************************************/
#
#	Adding source, e.g. file.cpp/h by:
#		add file.o to OBJECTS
#		add file.h to INCLUDES
#		add rule for file.o that depends on file.cpp AND all includes
#*****************************************************************************/
CPP = g++	
CFLAGS = -std=c++14 -g

OBJECTS = rp_server.o pitaya_interface.o misc.o rp_signal.o
INCLUDES = rsa.h misc.h const_strings.h rp_signal.h
SOURCES = rp_server.cpp rp_client.cpp rpcommand.cpp pitaya_interface.cpp
LDFLAGS = -pthread


all: rp_server rp_client

rp_signal.o : rp_signal.cpp $(INCLUDES)
	$(CPP) -c $(CFLAGS) $< -o $@

misc.o : misc.cpp $(INCLUDES)
	$(CPP) -c $(CFLAGS) $< -o $@

pitaya_interface.o : pitaya_interface.cpp pitaya_interface.h Makefile
	$(CPP) -c $(CFLAGS) $< -o $@

rp_server.o : rp_server.cpp $(INCLUDES) Makefile
	$(CPP) -c $(CFLAGS) $< -o $@
#.cpp.o:
#	$(CPP) -c $(CFLAGS) $< -o $@

rp_server: $(OBJECTS)
	$(CPP) -g $(OBJECTS) $(LDFLAGS) -o $@

rp_client: rp_client.cpp misc.cpp $(INCLUDES) Makefile
	$(CPP) $(CFLAGS) $(LDFLAGS) misc.o -o $@ $<
clean:
	rm *.o

clean_all:
	rm *.o rp_client rp_server

