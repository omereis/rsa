#ifndef	__RPCommand_H
#define	__RPCommand_H

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

//#include <nlohmann/json.hpp>
//using json = nlohmann::json;

#include "redpitaya/include/rp.h"

#include "rsa.h"
//enum ERPCommand {ERPC_SAMPLING, ERPC_ERPC_GET_SETUP, ERPC_SET_TRIGGER, ERPC_GET_SIGNALS, ERPC_GET_SAMPLES, ERPC_NONE};
using namespace rapidjson;
using namespace std;
//-----------------------------------------------------------------------------
class TPitayaInterface { // Res Pitaya Commands
public:
	TPitayaInterface ();
	TPitayaInterface (Document &document);
	~TPitayaInterface ();
	bool FollowCommand (const Document &docCommand, std::string &strReply);
protected:
	bool SetTrigger (const Value &valTrigger, std::string &strReply);
	bool SetTriggerLevel (const string &strLevel, string &strReply);
	bool SetTriggerLevel (const Value &valLevel, std::string &strReply);
	bool ExtractValueReal (const Value &val, double &dValue);
	bool ExtractValueString (const Value &val, std::string &str);
	bool UpdateTriggerItem (const char *szTriggerItem, const double &dValue);
	bool UpdateTriggerItem (const char *szTriggerItem, const std::string &strLevel);
private:
	Document m_document;
	
};
//-----------------------------------------------------------------------------
#endif // __RPCommand_H
