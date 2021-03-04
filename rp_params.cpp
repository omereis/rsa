;;/******************************************************************************\
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
	m_rate       = RP_SMP_125M;
	m_channel    = RP_CH_1;
	m_decimation = RP_DEC_1;
}
//-----------------------------------------------------------------------------

bool TRedPitayaParams::LoadFromJson (const std::string &strJson)
{
	return (true);
}
//-----------------------------------------------------------------------------

void TRedPitayaParams::GetTrigger (Json::Value &jvalSetup)
{
	Json::Value jvalTrigger;

	m_trigger.GetJson (jvalTrigger);
	jvalSetup["trigger"] = jvalTrigger;
}
//-----------------------------------------------------------------------------

bool TRedPitayaParams::SetTrigger (const Json::Value &jsonTrigger, std::string &strReply)
{
	bool f;
	Json::Value json;
	
	if ((f= m_trigger.SetTrigger (jsonTrigger)) == true) {
		GetTrigger (json);
		strReply = StringifyJson (json);
	}
	return (f);
}
//-----------------------------------------------------------------------------

rp_acq_sampling_rate_t TRedPitayaParams::GetRate() const
{
	return (m_rate);
}
//-----------------------------------------------------------------------------

void TRedPitayaParams::SetRate (rp_acq_sampling_rate_t rate)
{
	m_rate = rate;
}

//-----------------------------------------------------------------------------
rp_channel_t TRedPitayaParams::GetChannel () const
{
	return (m_channel);
}

//-----------------------------------------------------------------------------
void TRedPitayaParams::SetChannel (rp_channel_t channel)
{
	m_channel = channel;
}
//-----------------------------------------------------------------------------

rp_acq_decimation_t TRedPitayaParams::GetDecimation () const
{
	return (m_decimation);
}
//-----------------------------------------------------------------------------

void TRedPitayaParams::GetDecimation (rp_acq_decimation_t decimation)
{
	m_decimation = decimation;
}
//-----------------------------------------------------------------------------

/*
string  TRedPitayaParams::GetDecimationName () const
{
	return (GetDecimationName (m_decimation));
}
*/
//-----------------------------------------------------------------------------

string TRedPitayaParams::GetRateName () const
{
	return (SamplingRateName (GetRate()));
}
//-----------------------------------------------------------------------------

void TRedPitayaParams::SetRate (const string &strRate)
{
	SetRate(SamplingRateFromName(strRate));
}
//-----------------------------------------------------------------------------

string TRedPitayaParams::GetChannelName () const
{
	return (::GetChannelName (m_channel));
}
//-----------------------------------------------------------------------------
void TRedPitayaParams::SetChannel (const string &strChannel)
{
	m_channel = SetChannelFromName (strChannel);
}
//-----------------------------------------------------------------------------

bool TRedPitayaParams::GetSamplingParams (std::string &strReply)
{
	bool f;
	Json::Value jParams;
	
	try {
		strReply = "";
		if ((f = GetParams (jParams)) == true)
			strReply = StringifyJson (jParams);
		f = true;
	}
	catch (std::exception &e) {
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

bool TRedPitayaParams::GetTriggerAsString (std::string &strReply) const
{
	bool f;

	try {
		strReply = StringifyJson(m_trigger.AsJson ());
		f = true;
	}
	catch (std::exception &e) {
		strReply = e.what();
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

bool TRedPitayaParams::GetParams (Json::Value &jParams) const
{
	bool f;

	try {
		jParams[RPSU_RATE] = GetRateName ();
		jParams[RPSU_CHANNEL] = GetChannelName ();
		jParams[RPSU_DECIMATION] = GetDecimationName (m_decimation);
		jParams[RPSU_TRIGGER] = m_trigger.AsJson ();
		f = true;
	}
	catch (std::exception ) {
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

bool TRedPitayaParams::SetSamplingParams (const Json::Value &jParams, std::string &strReply)
{
	strReply = "TRedPitayaParams::SetSamplingParams";
	return (true);
}
//-----------------------------------------------------------------------------
