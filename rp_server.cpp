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

#include "pitaya_interface.h"

#include "rsa.h"
//#include "rsa_types.h"

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

bool ActOnCommand (const std::string &strJson, Document &docCommand, string &strReply)
{
	bool f;

	try {
		assert(docCommand.IsObject());
		TPitayaInterface pi(docCommand);
		//f = pi.FollowCommand (strJson, strReply);
		f = pi.FollowCommand (docCommand, strReply);
		//f = true;
/*
		if (document.HasMember("command")) {
			printf ("'command' found\n");
			const Value &val = document["command"];
			if (val.IsString())
				printf ("Command value is '%s'\n", val.GetString());
			else
				printf ("Command is not a string\n");
		}
*/
	}
	catch (exception &e) {
		printf ("ActOnCommand Runtime error:\n%s\n", e.what());
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

string ToLower (const std::string &str)
{
	string strLower;

	for (int n=0 ; n < str.size() ; n++)
		strLower += tolower(str[n]);
	return (strLower);
}

int main(int argc, char const *argv[])
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	char buffer[COMM_BUFFER_LENGTH];
	bool fParseError;
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
		memset (buffer, 0, COMM_BUFFER_LENGTH);
		valread = read( new_socket , buffer, COMM_BUFFER_LENGTH);
		try {
			string strMessage (buffer);
			string strJson = ToLower (strMessage);
			//if ((fParseError = document.Parse(buffer).HasParseError()) == false)
			if ((fParseError = document.Parse(strJson.c_str()).HasParseError()) == false)
				if (!ActOnCommand (strJson, document, strReply))
					valread = 0;
			//fParse = reader.parse (buffer, root);
		}
		catch (std::exception &e) {
			fprintf (stderr, "Parsing error:\n%s\n", e.what());
		}
		if (fParseError)
			strReply += "\nText to JSON Parsing fail";
		else
			strReply += "\nText to JSON Parsing OK";
		printf ("New Message of %d bytes\n", valread);
		printf ("JSON Parsing: %s\n", strReply.c_str());
		printf ("-------------------------------------------------------------------------------\n");
		//printf("%s\n", buffer);
		//printf ("-------------------------------------------------------------------------------\n");
		//printf("Message recieved: %s, valread=%d\n", buffer, valread);
		//sprintf (szHello, "Server Hello #%d", n++);
		send (new_socket , strReply.c_str(), strReply.size() ,0);
		//printf("Hello message sent\n");
	} while (valread > 0);
	return 0;
}

