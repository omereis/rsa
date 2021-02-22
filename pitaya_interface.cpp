/******************************************************************************\
|                            pitaya_interface.cpp                              |
\******************************************************************************/

#include <stdlib.h>
#include "pitaya_interface.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "redpitaya/include/rp.h"

#include "rsa.h"
#include "misc.h"
#include "const_strings.h"

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
	bool f = false;
	
	try {
		if (docCommand.HasMember (RPSU_TRIGGER)) {
			const Value &valTrigger = docCommand[RPSU_TRIGGER];
			f = SetTrigger(valTrigger, strReply);
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
	std::string str;
	bool fRead=false;
	double dValue;

	fRead = UpdateTriggerValueItem(valTrigger, RPSU_DELAY, EVT_REAL);
	fRead != UpdateTriggerValueItem(valTrigger, RPSU_LEVEL, EVT_REAL);
	fRead != UpdateTriggerValueItem(valTrigger, RPSU_SOURCE, EVT_STRING);
	fRead != UpdateTriggerValueItem(valTrigger, RPSU_DIR, EVT_STRING);
	return (fRead);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::UpdateTriggerValueItem(const Value &valTrigger, const char *szItem, EValueType type)
{
	std::string str;
	bool fRead=false;
	double dValue;

	if (valTrigger.HasMember(szItem)) {
		if (type == EVT_REAL) {
			if ((fRead = ExtractValueReal (valTrigger[szItem], dValue)) == true)
				fRead = UpdateTriggerItem (szItem, dValue);
		}
		else if (type == EVT_STRING) {
			if ((fRead = ExtractValueString (valTrigger[szItem], str)) == true)
				fRead = UpdateTriggerItem (szItem, str);
		}
	}
	return (fRead);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::ExtractValueReal (const Value &val, double &dValue)
{
	bool fRead = true;


	dValue = 0.0;
	if (val.IsString())
		dValue = atof (val.GetString());
	else if (val.IsDouble())
		dValue = val.GetDouble();
	else if (val.IsInt())
		dValue = (double) val.GetInt();
	else
		fRead = false;
	return (fRead);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::ExtractValueString (const Value &val, std::string &str)
{
	bool fRead = true;

	str = "";
	if (val.IsString())
		str = val.GetString();
	return (fRead);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::UpdateTriggerItem (const char *szTriggerItem, const double &dValue)
{
	Document docSetup;
	bool fUpdate = false;
	
	if (ReadPitayaSetup (docSetup)) {
		Value &valTrigger = docSetup[RPSU_TRIGGER];
		Value &valLevel = valTrigger[szTriggerItem];
		valLevel.SetDouble (dValue);
		fUpdate = WriteJson (docSetup);
	}
	return (fUpdate);
}

//-----------------------------------------------------------------------------

bool TPitayaInterface::UpdateTriggerItem (const char *szTriggerItem, const std::string &str)
{
	Document docSetup;
	bool fUpdate = false;
	
	if (ReadPitayaSetup (docSetup)) {
		Value &valTrigger = docSetup[RPSU_TRIGGER];
		Value &valLevel = valTrigger[szTriggerItem];
		valLevel.SetString (str.c_str(), str.size());
		fUpdate = WriteJson (docSetup);
	}
	return (fUpdate);
}

//-----------------------------------------------------------------------------
