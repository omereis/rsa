/******************************************************************************\
|                                rp_params.cpp                                 |
\******************************************************************************/
#include "rp_params.h"
#include "rsa.h"
#include "const_strings.h"

TRedPitayaParams::TRedPitayaParams ()
{
	Clear ();
}
//-----------------------------------------------------------------------------

bool TRedPitayaParams::Clear ()
{
	m_trigger.Clear ();
}
//-----------------------------------------------------------------------------

bool TRedPitayaParams::LoadFromJson (const std::string &strJson)
{
	return (true);
/*
	bool fLoad;

	if ((fLoad = LoadFromJson (PITAYA_MOCK_NAME)) == true) {
	}
	return (fLoad);
*/
}

/*
bool TRedPitayaParams::LoadFromJson (const std::string &strJson)
{
	bool fLoad;
	Document docSetup;

	if ((fLoad = ReadPitayaSetup (docSetup)) == true)
		if (docSetup.HasMember ("trigger"))
			fLoad = m_trigger.LoadFromJson (docSetup["trigger"]);
}
*/
//-----------------------------------------------------------------------------

/*
bool TRedPitayaParams::GetTrigger (Document &docTrigger)
{
	return (m_trigger.JsonTrigger (docTrigger));
}
*/
//-----------------------------------------------------------------------------

/*
bool TRedPitayaParams::GetSetup (const Value &valSetup, std::string &strReply)
{
	bool f;

	try {
		const char *szCommand = valSetup.GetString();
		if (strcmp (szCommand, RPSU_GET_TRIGGER) == 0) {
			Document docTrigger;
			docTrigger.SetObject();
			m_trigger.JsonTrigger (docTrigger);
			**
			std::string strLevel ("level");

			docTrigger.SetObject();
			Value val (strLevel.c_str(), strLevel.size(), docTrigger.GetAllocator());
			docTrigger.AddMember (val, 1e-5, docTrigger.GetAllocator());
			**

			StringBuffer sb;
			Writer<StringBuffer> writer(sb);
			docTrigger.Accept (writer);
			const char *szReply = sb.GetString();
			printf ("%s\n", szReply);
			strReply = std::string (sb.GetString());
		}
		f = true;
	}
	catch (std::exception &e) {
		f = false;
		strReply = e.what();
	}
	return (f);
}
*/
//-----------------------------------------------------------------------------

void TRedPitayaParams::GetTrigger (Json::Value &jvalSetup)
{
	Json::Value jvalTrigger;

	m_trigger.GetJson (jvalTrigger);
	jvalSetup["trigger"] = jvalTrigger;
}
