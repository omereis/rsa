/*
pitaya_interface.cpp
*/


#include <stdlib.h>
#include "pitaya_interface.h"

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "redpitaya/include/rp.h"

#include "rsa.h"
#include "misc.h"
#include "const_strings.h"

//extern const std::string g_strTrigger;
//extern const std::string g_strLevel;

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
		//if (docCommand.HasMember ("trigger")) {
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

	if (valTrigger.HasMember(RPSU_DELAY)) {
		if ((fRead = ExtractValueReal (valTrigger[RPSU_DELAY], dValue)) == true)
			fRead = UpdateTriggerItem (RPSU_DELAY, dValue);
	}
	if (valTrigger.HasMember(RPSU_LEVEL)) {
		if ((fRead = ExtractValueReal (valTrigger[RPSU_LEVEL], dValue)) == true)
			fRead = UpdateTriggerItem (RPSU_LEVEL, dValue);
	}
	if (valTrigger.HasMember(RPSU_SOURCE)) {
		if ((fRead = ExtractValueString (valTrigger[RPSU_SOURCE], str)) == true)
			fRead = UpdateTriggerItem (RPSU_SOURCE, str);
	}
	if (valTrigger.HasMember(RPSU_DIR)) {
		if ((fRead = ExtractValueString (valTrigger[RPSU_DIR], str)) == true)
			fRead = UpdateTriggerItem (RPSU_DIR, str);
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
	//if (strLevel.size() > 0)
		//fRead = SetTriggerLevel (strLevel, strReply);
	//else
		//fRead = false;
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
