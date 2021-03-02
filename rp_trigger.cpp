/******************************************************************************\
|                                rp_trigger.cpp                                |
\******************************************************************************/
#include "rp_trigger.h"
#include "const_strings.h"

//-----------------------------------------------------------------------------
TRedPitayaTrigger::TRedPitayaTrigger ()
{
	Clear ();
}
//-----------------------------------------------------------------------------

TRedPitayaTrigger::TRedPitayaTrigger (const TRedPitayaTrigger &other)
{
	AssignAll (other);
}
//-----------------------------------------------------------------------------

TRedPitayaTrigger TRedPitayaTrigger::operator= (const TRedPitayaTrigger &other)
{
	AssignAll (other);
	return (*this);
}
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::operator== (const TRedPitayaTrigger &other) const
{
	if (GetLevel() != other.GetLevel())
		return (false);
	if (GetDelay() != other.GetDelay ())
		return (false);
	if (GetInput() != other.GetInput())
		return (false);
	if (GetDirection () != other.GetDirection())
		return (false);
	return (true);
}
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::operator!= (const TRedPitayaTrigger &other) const
{
	return (!(*this == other));
}
//-----------------------------------------------------------------------------

void TRedPitayaTrigger::Clear ()
{
	SetLevel (5e-3);
	SetDelay (100e6);
	m_source = RP_TRIG_SRC_CHA_PE;
	m_channel = RP_CH_1;
	//SetInput ("in1");
	//SetDirection ("rise");
}
//-----------------------------------------------------------------------------

/*
bool TRedPitayaTrigger::LoadFromJson (const Value &valTrigger)
{
	if (valTrigger.HasMember (RPSU_LEVEL))
		SetLevel(valTrigger[RPSU_LEVEL].GetDouble());
}
*/
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::LoadFromJson (const std::string &strFile)
{
}
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::SaveToJson  (const std::string &strFile)
{
}
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::LoadFromCard ()
{
}
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::UpdateInCard ()
{
}
//-----------------------------------------------------------------------------
	
float TRedPitayaTrigger::GetLevel () const
{
	return (m_fLevel);
}
//-----------------------------------------------------------------------------

void TRedPitayaTrigger::SetLevel (float fLevel)
{
	m_fLevel = fLevel;
}
//-----------------------------------------------------------------------------

float TRedPitayaTrigger::GetDelay ()const
{
	return (m_fDelay);
}
//-----------------------------------------------------------------------------

void TRedPitayaTrigger::SetDelay (float fDelay)
{
	m_fDelay = fDelay;
}
//-----------------------------------------------------------------------------

std::string TRedPitayaTrigger::GetInput () const
{
	return (TriggerSourceName (m_source));
}
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::SetInput (const std::string &strIn)
{
	return (SetTriggerFromName (strIn, GetDirection (), m_source));
}
//-----------------------------------------------------------------------------

std::string TRedPitayaTrigger::GetDirection () const
{
	return (TriggerDirName (m_source));
}
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::SetDirection (const std::string strDir)
{
	return (SetTriggerFromName (GetInput (), strDir, m_source));
}
//-----------------------------------------------------------------------------

void TRedPitayaTrigger::AssignAll (const TRedPitayaTrigger &other)
{
	SetLevel (other.GetLevel());
	SetDelay (other.GetDelay ());
	SetInput (other.GetInput());
	SetDirection (GetDirection());
}
//-----------------------------------------------------------------------------

/*
bool  TRedPitayaTrigger::JsonTrigger (Document &docTrigger)
{
	Value val (RPSU_LEVEL, strlen(RPSU_LEVEL), docTrigger.GetAllocator());
	docTrigger.AddMember(val, GetLevel(), docTrigger.GetAllocator());
	Value val1 (RPSU_DELAY, strlen(RPSU_DELAY), docTrigger.GetAllocator());
	docTrigger.AddMember (val1, GetDelay(), docTrigger.GetAllocator());
	return (true);
}
*/
//-----------------------------------------------------------------------------

void TRedPitayaTrigger::GetJson (Json::Value &valTrigger)
{
	//valTrigger.Clear();
	valTrigger[RPSU_LEVEL] = GetLevel();
	valTrigger[RPSU_DELAY] = GetDelay();
	valTrigger[RPSU_SOURCE] = GetInput();
	valTrigger[RPSU_DIR] = GetDirection ();
}
//-----------------------------------------------------------------------------

bool TRedPitayaTrigger::SetTrigger (const Json::Value &valTrigger)
{
	bool f;

	try {
		if (!valTrigger[RPSU_LEVEL].isNull())
			SetLevel(valTrigger[RPSU_LEVEL].asFloat());
		if (!valTrigger[RPSU_DELAY].isNull())
			SetDelay (valTrigger[RPSU_DELAY].asFloat());
		if (!valTrigger[RPSU_SOURCE].isNull())
			SetInput(valTrigger[RPSU_SOURCE].asString());
		if (!valTrigger[RPSU_DIR].isNull())
			SetDirection (valTrigger[RPSU_DIR].asString());
		f = true;
	}
	catch (std::exception &e) {
		f = false;
		fprintf (stderr, "Runtime error in 'TRedPitayaTrigger::SetJson':\n%s\n", e.what());
	}
	return (f);
}
//-----------------------------------------------------------------------------
