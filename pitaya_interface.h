#ifndef	__RPCommand_H
#define	__RPCommand_H

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

#include "redpitaya/include/rp.h"

#include "rsa.h"

using namespace rapidjson;
using namespace std;
enum EValueType {EVT_NONE, EVT_STRING, EVT_REAL};
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
	bool UpdateTriggerValueReal(const Value &valTrigger, const char *szItem);
	bool UpdateTriggerValueItem(const Value &valTrigger, const char *szItem, EValueType type);
private:
	Document m_document;
	
};
//-----------------------------------------------------------------------------
#endif // __RPCommand_H
