// Server side C/C++ program to demonstrate Socket programming 
#include <mutex>

#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
/*
#include <jsoncpp/json/json.h>
#include <jsoncpp/json/reader.h>
#include <jsoncpp/json/writer.h>
#include <jsoncpp/json/value.h>
*/

//#define PORT 5500

#include "comm.h"
#include "rsa_types.h"

using namespace rapidjson;
using namespace std;

//-----------------------------------------------------------------------------
int GetCliPort (int argc, char const *argv[])
{
	int nPort;
	
	if (argc > 1)
		nPort = atoi (argv[1]);
	else
		nPort = DEFAULT_PORT;
	return (nPort);
}
//-----------------------------------------------------------------------------

int OpenSocket (int nPort)
{
	struct sockaddr_in address; 
	int opt = 1; 
	int nServerFd;

	// Creating socket file descriptor 
	if ((nServerFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 

	// Forcefully attaching socket to the port
	if (setsockopt(nServerFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))  { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(nPort); 

	// Forcefully attaching socket to the port
	if (bind(nServerFd, (struct sockaddr *)&address, sizeof(address)) < 0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(nServerFd, 3) < 0)  { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	}
	return (nServerFd);
}
//-----------------------------------------------------------------------------

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	bool fParse;
	int nPort;// = DEFAULT_PORT;
	TStringQueue qCommand, qReply;
	std::mutex mutex;
	string strReply;
	Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.

	nPort = GetCliPort (argc, argv);
	server_fd = OpenSocket (nPort);
	char szHello[1024];

	int n=1;
	do {
		fprintf (stderr, "Waiting at port %d\n", nPort);
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){ 
			perror("accept");
			exit(EXIT_FAILURE);
		}
		memset (buffer, 0, sizeof (buffer));
		valread = read( new_socket , buffer, 1024);
		try {
			fParse = document.Parse(buffer).HasParseError();
			//fParse = reader.parse (buffer, root);
		}
		catch (std::exception &e) {
			fprintf (stderr, "Parsing error:\n%s\n", e.what());
		}
		if (fParse)
			strReply = "Text to JSON Parsing OK";
		else
			strReply = "Text to JSON Parsing fail";
		printf ("New Message of %d bytes\n", valread);
		printf ("-------------------------------------------------------------------------------\n");
		printf("%s\n", buffer);
		printf ("-------------------------------------------------------------------------------\n");
		//printf("Message recieved: %s, valread=%d\n", buffer, valread);
		printf ("JSON Parsing: %s\n", strReply.c_str());
		sprintf (szHello, "Server Hello #%d", n++);
		send (new_socket , strReply.c_str(), strReply.size() ,0);
		printf("Hello message sent\n");
	} while (valread > 0);
	return 0;
}

