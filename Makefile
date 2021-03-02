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

OBJECTS = rp_server.o pitaya_interface.o misc.o rp_signal.o rp_trigger.o rp_params.o
INCLUDES = rsa.h misc.h const_strings.h rp_signal.h rp_trigger.h rp_params.h
SOURCES = rp_server.cpp rp_client.cpp rpcommand.cpp pitaya_interface.cpp
LDFLAGS = -pthread -ljsoncpp


all: rp_server rp_client

rp_params.o : rp_params.cpp $(INCLUDES)
	$(CPP) -c $(CFLAGS) $< -o $@

rp_trigger.o : rp_trigger.cpp $(INCLUDES)
	$(CPP) -c $(CFLAGS) $< -o $@

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

rp_client.o : rp_client.cpp misc.cpp misc.h Makefile
	$(CPP) -c $(CFLAGS) $< -o $@

rp_client: rp_client.o misc.o $(INCLUDES) Makefile
	$(CPP) $(CFLAGS) misc.o -o $@ $< $(LDFLAGS) 
clean:
	rm *.o

clean_all:
	rm *.o rp_client rp_server

