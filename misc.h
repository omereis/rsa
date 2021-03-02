#ifndef	__MISC_H_INC
#define	__MISC_H_INC

#include "rp.h"
#include "jsoncpp/json/json.h"

#include <sys/socket.h> 
#include <netinet/in.h> 

#include <string>

//#include "rapidjson/document.h"     // rapidjson's DOM-style API
//#include "rapidjson/prettywriter.h" // for stringify JSON

//enum ERPCommand {ERPC_SAMPLING, ERPC_ERPC_GET_SETUP, ERPC_SET_TRIGGER, ERPC_GET_SIGNALS, ERPC_GET_SAMPLES, ERPC_NONE};
//using namespace rapidjson;
using namespace std;
//-----------------------------------------------------------------------------
bool ReadFile (const std::string &strFileName, std::string &strContent);
//bool ReadPitayaSetup (Document &docSetup);
//bool ReadPitayaSetup (std::string &strJson, Document &docSetup);

bool ReadPitayaSetup (Json::Value &valSetup);
bool ReadPitayaSetup (std::string &strJson, Json::Value &valSetup);

//bool WriteJson (Document &doc);
int OpenServerSocket (int nPort);
bool IsJsonQuit (const char *sz);
std::string ToLower (const std::string &str);
std::string TriggerSourceName (const rp_acq_trig_src_t &source);
bool SetTriggerFromName (const std::string &strSourceIn, const std::string &strSourceDir, rp_acq_trig_src_t & source);
std::string TriggerDirName (const rp_acq_trig_src_t &source);
std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& rtrim(std::string& str, const std::string& chars = "\t\n\v\f\r ");
std::string& trim(std::string& str, const std::string& chars = "\t\n\v\f\r ");

bool GetJsonInt (const Json::Value &val, const std::string &strKey, int &nValue);
bool ReadPitayaSetup (Json::Value &valSetup);
std::string StringifyJson (const Json::Value &val);
//-----------------------------------------------------------------------------
#endif //__MISC_H_INC
