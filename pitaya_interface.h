#ifndef	__RPCommand_H
#define	__RPCommand_H

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON

#include "redpitaya/include/rp.h"

#include "rsa_types.h"
//enum ERPCommand {ERPC_SAMPLING, ERPC_ERPC_GET_SETUP, ERPC_SET_TRIGGER, ERPC_GET_SIGNALS, ERPC_GET_SAMPLES, ERPC_NONE};
using namespace rapidjson;
using namespace std;
//-----------------------------------------------------------------------------
class TPitayaInterface { // Res Pitaya Commands
public:
	TPitayaInterface ();
	TPitayaInterface (Document &document);
	~TPitayaInterface ();
	bool FollowCommand (const std::string &str, Document &docCommand, string &strReply);
private:
	Document m_document;
	
};
//-----------------------------------------------------------------------------
#endif // __RPCommand_H
