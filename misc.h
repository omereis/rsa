#ifndef	__MISC_H_INC
#define	__MISC_H_INC

#include <sys/socket.h> 
#include <netinet/in.h> 

#include <string>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

//enum ERPCommand {ERPC_SAMPLING, ERPC_ERPC_GET_SETUP, ERPC_SET_TRIGGER, ERPC_GET_SIGNALS, ERPC_GET_SAMPLES, ERPC_NONE};
using namespace rapidjson;
using namespace std;
//-----------------------------------------------------------------------------
bool ReadFile (const std::string &strFileName, std::string &strContent);
bool ReadPitayaSetup (Document &docSetup);
bool WriteJson (Document &doc);
int OpenServerSocket (int nPort);
bool IsJsonQuit (const char *sz);

//-----------------------------------------------------------------------------
#endif //__MISC_H_INC
