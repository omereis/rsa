/*
pitaya_interface.cpp
*/


#include <stdlib.h>
#include "pitaya_interface.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "redpitaya/include/rp.h"

#include "rsa.h"
//#include "rsa_types.h"
#include "misc.h"

using namespace rapidjson;
using namespace std;
//-----------------------------------------------------------------------------
TPitayaInterface::TPitayaInterface ()
{
}
//-----------------------------------------------------------------------------
TPitayaInterface::TPitayaInterface (Document &document)
{
	Document::AllocatorType& a = document.GetAllocator();

	m_document.CopyFrom (document, a);
}
//-----------------------------------------------------------------------------
TPitayaInterface::~TPitayaInterface ()
{
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::FollowCommand (const Document &docCommand, std::string &strReply)
{
	string strCommand;
	bool f;
	
	try {
		if (docCommand.HasMember ("trigger")) {
			const Value &valTrigger = docCommand["trigger"];
			SetTrigger(valTrigger, strReply);
			//SetTrigger(docCommand["trigger"]);
		}
	}
	catch (exception &e) {
			f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::SetTrigger (const Value &valTrigger, string &strReply)
{
	std::string strLevel;
	bool fRead;

	if (valTrigger.HasMember("level")) {
		const Value &valLevel = valTrigger["level"];
		if (valLevel.IsString())
			strLevel = valLevel.GetString();
		else if (valLevel.IsDouble()) {
			printf ("value is double\n");
			strLevel = to_string(valLevel.GetDouble());
		}
		if (strLevel.size() > 0)
			fRead = SetTriggerLevel (strLevel, strReply);
		else
			fRead = false;
	}
	return (fRead);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::SetTriggerLevel (const std::string &strLevel, std::string &strReply)
{
	Document docSetup;
	
	if (ReadPitayaSetup (docSetup)) {
		Value &valTrigger = docSetup["trigger"];
		Value &valLevel = valTrigger["level"];
		valLevel.SetDouble (std::stod(strLevel));
		WriteJson (docSetup);
	}
	strReply = strLevel;
	return (true);
}
//-----------------------------------------------------------------------------
