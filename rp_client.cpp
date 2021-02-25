// Client side C/C++ program to demonstrate Socket programming 
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>

#include <mutex>
#include <thread>

#include <string>
#include <iostream>
#include <fstream>

#include "rsa.h"
#include "misc.h"

using namespace std;

static std::mutex mutexQuitSender;
static bool s_fQuitSender = false;
//-----------------------------------------------------------------------------
std::string FindHostIPByName (const std::string &strHostName)
{
	hostent* hostname = gethostbyname(strHostName.c_str());
	std::string strAddress;

	if(hostname)
		strAddress = std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
	return (strAddress);
}
//-----------------------------------------------------------------------------

int SafeReadCliPort (const char *szPort)
{
	int nPort;

	try {
		nPort = atoi(szPort);
	}
	catch (const std::exception &e){
		nPort = DEFAULT_PORT;
	}
	return (nPort);
}
//-----------------------------------------------------------------------------

bool GetCliAddressPort (int argc, char const *argv[], int &nPort, string &strAddress, string &strErr)
{
	bool f;
		
	try {
		nPort = DEFAULT_PORT;
		strAddress = DEFAULT_ADDRESS;
		if (argc > 1)
			nPort = SafeReadCliPort (argv[1]);
		if (argc > 2)
			strAddress = FindHostIPByName (argv[2]);
			//strAddress = argv[2];
		f = true;
	}
	catch (exception &e){
		nPort = DEFAULT_PORT;
		strAddress = DEFAULT_ADDRESS;
		strErr = e.what();
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

std::string GetResultFileName()
{
	std::string strName, strBase ("remote"), strExt("csv");
	FILE *file;
	char szBuf[100];

	int n=0;
		while (strName.size() == 0) {
		sprintf (szBuf, "%s%02d.%s", strBase.c_str(), n++, strExt.c_str());
		file = fopen (szBuf, "r");
		if (file == NULL)
			strName = std::string (szBuf);
		else
			fclose (file);
	}
	return (strName);
}
//-----------------------------------------------------------------------------

void ReadSamples (int nPort, int nSampleLen)
{
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int addrlen = sizeof(address);
	int opt = 1; 
	int nBufferSize = nSampleLen * sizeof (float);
	char *buffer = new char [nSampleLen * sizeof (float)];
	bool fQuit;
	
	mutexQuitSender.lock();
	fQuit = s_fQuitSender = false;
	mutexQuitSender.unlock();

		//fd_set set;
		//struct timeval timeout;
		//FD_ZERO(&set); /* clear the set */
		//FD_SET(new_socket, &set); /* add our file descriptor to the set */
		//timeout.tv_sec = 1;
		//timeout.tv_usec = 0;
		//int rv = select(new_socket, &set, NULL, NULL, &timeout);
	server_fd = OpenServerSocket (nPort);
	while (!fQuit) {
		if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0){ 
			perror("accept");
			exit(EXIT_FAILURE);
		}
		memset (buffer, 0, nBufferSize);
		valread = read(new_socket, buffer, nBufferSize);

		if (valread > 0) {
/*
			FILE *f = fopen ("debug.txt", "a+");
			fprintf (f, "\n%s\n", buffer);
			fclose (f);
*/
			if (IsJsonQuit (buffer))
				fQuit = true;
			else {
				float *pNum = (float*) buffer;
				TFloatVector vf;
				int n=0;
				while (n < valread) {
					vf.push_back (*pNum);
					pNum ++;
					n += sizeof (*pNum);
				}
				if (vf.size() > 0) {
					TFloatVector::iterator i;
					std::string strFileName = GetResultFileName();
					FILE *file = fopen ("remote_data.txt", "a+");
					//FILE *file = fopen (strFileName.c_str(), "w+");
					fprintf (file, "-----------------------------------------------------\n");
					for (i=vf.begin() ; i != vf.end() ; i++)
						fprintf (file, "%g\n", *i);
					fprintf (file, "-----------------------------------------------------\n");
					fclose (file);
				}
			}
		}
		else {
			valread = 0;
		}
		send (new_socket, "OK", 2, 0);
		if (!fQuit) {
			mutexQuitSender.lock();
			fQuit = s_fQuitSender;
			mutexQuitSender.unlock();
		}
	}

	delete[] buffer;
}
//-----------------------------------------------------------------------------

int SendData (const std::string &strAddress, int nPort, const std::string &strData)
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	const char *hello = "Hello from client";
	char buffer[COMM_BUFFER_LENGTH] = {0};
	string strErr;

	printf ("Conecting to server %s:%d\n", strAddress.c_str(), nPort);
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(nPort);
	// Convert IPv4 and IPv6 addresses from text to binary form 
	if(inet_pton(AF_INET, strAddress.c_str(), &serv_addr.sin_addr) <=0 ) {
		//if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <=0 ) {
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}
	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) { 
		printf("\nConnection Failed \n");
		return -1;
	}
	send (sock, strData.c_str(), strData.size(), 0);
	printf("Message sent: '%s'\n", strData.c_str());
	//valread = read( sock , buffer, COMM_BUFFER_LENGTH);
	printf("Server answer is: '%s'\n", buffer);
}

const char *aszMainMenu[] = {
		"1. Compose Message",
		"2. JSON File",
		"3. Quite",
		""
	};
//-----------------------------------------------------------------------------

void PrintMenu()
{
	int n=0;
	const char *sz;
	
	do {
		sz = aszMainMenu[n++];
		printf ("%s\n", sz);
	} while (strlen(sz) > 0);
}
//-----------------------------------------------------------------------------

std::string GetFileName ()
{
	std::string strFileName;
	
	printf ("Enter File name [.json], plase... ");
	getline (cin, strFileName);
	if (strFileName.find (".") == string::npos)
		strFileName += ".json";
	return (strFileName);
}
//-----------------------------------------------------------------------------

std::string ReadJSONFile ()
{
	std::string strFileName, strLine, strData="";
	
	strFileName = GetFileName ();
	std::fstream file;// (strFileName);
	file.open (strFileName.c_str(), ios::in);
	if (file) {
		while (!file.eof()) {
			getline(file, strLine);
			strData += strLine + "\n";
		}
	}
	return (strData);
}
//-----------------------------------------------------------------------------

std::string GetDataToSend (bool &fToQuit)
{
	std::string strData;

	PrintMenu();
	fToQuit = false;
	getline (cin, strData);
	int nData = atoi (strData.c_str());
	if (nData == 1)
		strData = "Compose Message";
	else if (nData == 2)
		strData = ReadJSONFile ();//"JSON File";
	else {
		strData = "{\"quit\" : \"quit\"}";
		fToQuit = true;
	}
	return (strData);
}
//-----------------------------------------------------------------------------

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	const char *hello = "Hello from client";
	char buffer[COMM_BUFFER_LENGTH] = {0};
	int nPort;
	string strAddress, strErr, strData;
	bool fToQuit;

	if (GetCliAddressPort (argc, argv, nPort, strAddress, strErr)) {
		thread *pthReadSamples = new thread (ReadSamples, nPort + 1, 6250);
		printf ("Connecting to server at %s:%d\n", strAddress.c_str(), nPort);
		do {
			strData = GetDataToSend(fToQuit);
			SendData (strAddress, nPort, strData);
		} while (fToQuit == false);
		SendData ("127.0.0.1", nPort+1, strData);
		mutexQuitSender.lock();
		s_fQuitSender = true;
		mutexQuitSender.unlock();
		if (pthReadSamples->joinable())
			pthReadSamples->join();
			//delete pthReadSamples;
	}
	return 0;
} 
