/******************************************************************************\
|                               rp_server.cpp                                  |
\******************************************************************************/

// Server side C/C++ program to demonstrate Socket programming 
#include <mutex>
#include <thread>
#include <chrono>

#include<iostream>
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <string.h> 

//#include "rapidjson/document.h"     // rapidjson's DOM-style API
//#include "rapidjson/prettywriter.h" // for stringify JSON

#include "pitaya_interface.h"

#include "rsa.h"
#include "misc.h"

//using namespace rapidjson;
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

bool ActOnCommand (const std::string &strJson, TPitayaInterface &pi, Json::Value &root, const char *szClientIP, string &strReply)
//bool ActOnCommand (const std::string &strJson, TPitayaInterface &pi, Document &docCommand, const char *szClientIP, string &strReply)
{
	bool f;

	try {
		//assert(docCommand.IsObject());
		//TPitayaInterface pi(szClientIP);
		f = pi.FollowCommand (root, strReply);
		//f = pi.FollowCommand (docCommand, strReply);
	}
	catch (exception &e) {
		printf ("ActOnCommand Runtime error:\n%s\n", e.what());
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------
int SampleSignal (float *afSignals, int nLength)
{
	FILE *file = fopen ("signal.dat", "r");
	char szBuf[100], *sz;
	int n=0;
	float f;
	
	if (file != NULL) {
		while (fgets(szBuf, 100, file) != NULL) {
			afSignals[n++] = atof(szBuf);
			if (n >= nLength)
				break;
		}
	}
	fclose (file);
	return (n);
}
//-----------------------------------------------------------------------------
#include "rp_signal.h"
std::mutex mutexQueue;
queue<TRpSignal> qSignals;
queue<time_t> qMine;
std::mutex mutexStop;
bool g_fStop;
//-----------------------------------------------------------------------------
using namespace std::chrono_literals;
void SampleInput ()
{
	bool fContinue=true;
	float *afSignals = new float[MAX_BUFF_SIZE];
	int nSamples;

	while (fContinue) {
		if ((nSamples = SampleSignal (afSignals, MAX_BUFF_SIZE)) > 0) {
			TRpSignal signal (afSignals, nSamples);
			mutexQueue.lock();
			while (qSignals.size() >= MAX_QUEUE_LENGTH)
				qSignals.pop();
			qSignals.push (TRpSignal(signal));
			mutexQueue.unlock();
		}

		std::this_thread::sleep_for(2000ms);

		mutexStop.lock();
		fContinue = !g_fStop;
		mutexStop.unlock();
	}
	delete[] afSignals;
}
/*
sampler - read signals from input(file) & put in signals queue
psd - perform PSD analysis
mca - perform MCA analysis
*/
//-----------------------------------------------------------------------------

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
	std::thread threadSample (SampleInput);
	string strReply;
	//Document document;  // Default template parameter uses UTF8 and MemoryPoolAllocator.
	TPitayaInterface pi;
	Json::Value root;
	Json::Reader reader;

	nPort = GetCliPort (argc, argv);
	server_fd = OpenServerSocket (nPort);
	char szHello[1024];

	int n=1;
	g_fStop = false;
	pi.LoadSetup();
	do {
		fprintf (stderr, "Waiting at port %d\n", nPort);
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){ 
			perror("accept");
			exit(EXIT_FAILURE);
		}
		memset (buffer, 0, COMM_BUFFER_LENGTH);
		valread = read(new_socket, buffer, COMM_BUFFER_LENGTH);
		struct in_addr ipAddr = address.sin_addr;
		char szClientIP[INET_ADDRSTRLEN];
		inet_ntop(AF_INET, &ipAddr, szClientIP, INET_ADDRSTRLEN);
		printf ("Client address is %s\n", szClientIP);
		try {
			string strMessage (buffer);
			string strJson = ToLower (strMessage);
			if (reader.parse (strJson, root)) {
				if (!ActOnCommand (strJson, pi, root, szClientIP, strReply))
					valread = 0;
			}
/*
			if ((fParseError = document.Parse(strJson.c_str()).HasParseError()) == false) {
				pi.SetClientIP (szClientIP);
				//TPitayaInterface pi(szClientIP);
				if (!ActOnCommand (strJson, pi, document, szClientIP, strReply))
					valread = 0;
			}
*/
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
		send (new_socket , strReply.c_str(), strReply.size() ,0);
		for (int n=0 ; n < 10 ; n++) {
			mutexQueue.lock();
			if (!qMine.empty()) {
				cout << qMine.front() << "\n";
				qMine.pop();
			}
			mutexQueue.unlock();
		}
	} while (valread > 0);
	g_fStop = true;
	threadSample.join();
	//mutexStop.lock();
	//g_fStop = true;
	//mutexStop.unlock();
	return 0;
}

