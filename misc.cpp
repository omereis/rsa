/*
m i s c . c p p
*/


#include "misc.h"
#include "rsa.h"
#include <stdio.h>

#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include <cstdio>

using namespace rapidjson;
using namespace std;

//-----------------------------------------------------------------------------
bool ReadFile (const std::string &strFileName, std::string &strContent)
{
	bool fRead;
	std::string str;

	try {
		strContent = "";
		FILE *file;
		if ((file = fopen (strFileName.c_str(), "r")) != NULL) {
			char *szBuf = new char[4096];
			while (fgets(szBuf, 4000, file) != NULL)
				str += std::string(szBuf);
			fclose (file);
			delete[] szBuf;
			strContent = str;
		}
		fRead = true;
	}
	catch (std::exception &e) {
		fRead = false;
	}
	return (fRead);
}
//-----------------------------------------------------------------------------

bool ReadPitayaSetup (Document &docSetup)
{
	std::string str;
	bool fRead;

	try {
		if ((fRead = ReadFile (PITAYA_MOCK_NAME, str)) == true) {
			docSetup.Parse(str.c_str());
			fRead != docSetup.HasParseError();
		}
	}
	catch (std::exception &e){
		fRead = false;
	}
	return (fRead);
}
//-----------------------------------------------------------------------------

bool WriteJson (Document &doc)
{
	// example from https://rapidjson.org/md_doc_stream.html
	FILE *file;
	bool fWrite;
	std::string strFileName (PITAYA_MOCK_NAME);

	if ((file = fopen (strFileName.c_str(), "wb")) != NULL) {
		char *szWriteBuffer = new char [8192];
		FileWriteStream os(file, szWriteBuffer, sizeof(szWriteBuffer));
		Writer<FileWriteStream> writer(os);
		doc.Accept(writer);
		fclose (file);
		fWrite = true;
	}
	else
		fWrite = false;
	return (fWrite);
}
//-----------------------------------------------------------------------------

int OpenServerSocket (int nPort)
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
/*
#include <ctype.h>
static bool FindJsonStart (const char *sz, int &iStart)
{
	int n;
	
	iStart = -1;
	for (n=0 ; (n < nBufLen) && (iStart < 0) ; n++) {
		if (!isblank(sz[n])) {
			iStart
			if (sz == '{')
				iStart = n;
			else
				break;
		}
	}
	return (iStart >= 0);
}
*/
//-----------------------------------------------------------------------------
#include <strings.h>
bool IsJsonQuit (const char *szJson)
{
	Document document;
	const char *szQuit=NULL;
	bool fQuit=false;
	
	if (document.Parse(szJson).HasParseError() == false)
		if (document.HasMember("quit"))
			szQuit = document["quit"].GetString();
	if (szQuit != NULL)
		fQuit = (strcasecmp (szQuit, "quit") == 0);
	return (fQuit);
}
//-----------------------------------------------------------------------------
