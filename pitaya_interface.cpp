/*
RPCommand.cpp
*/

#include <nlohmann/json.hpp>

#include "pitaya_interface.h"

#include "redpitaya/include/rp.h"

#include "comm.h"
#include "rsa_types.h"

using namespace rapidjson;
using json = nlohmann::json;
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
bool TPitayaInterface::FollowCommand (const std::string &str, Document &docCommand, string &strReply)
{
	string strCommand;
	bool f;
	
	try {
		json jCommand = json::parse(str.c_str());
		if (jCommand.contains("quit"))
			f = false;
		else {
			if (jCommand.contains("trigger"))
				printf ("Lohmann found trigger\n");
			else
				printf ("Lohmann could NOT find  trigger\n");
			if (docCommand.HasMember("trigger"))
//int rp_AcqSetTriggerSrc(rp_acq_trig_src_t source);
//int rp_AcqGetTriggerSrc(rp_acq_trig_src_t* source);
// int rp_AcqSetTriggerDelay(int32_t decimated_data_num);
// int rp_AcqGetTriggerDelay(int32_t* decimated_data_num);
// int rp_AcqSetTriggerDelayNs(int64_t time_ns);
// int rp_AcqGetTriggerDelayNs(int64_t time_ns);
// int rp_AcqSetTriggerLevel(rp_channel_trigger_t channel, float voltage);
// int rp_AcqGetTriggerLevel(rp_channel_trigger_t channel, float* voltage);
// int rp_GenTriggerSource(rp_channel_t channel, rp_trig_src_t src);
// int rp_GenGetTriggerSource(rp_channel_t channel, rp_trig_src_t *src);
				printf ("Got trigger command\n");
			if (docCommand.HasMember("sampling"))
				printf ("Got sampling command\n");
			f = true;
		}
	}
	catch (exception &e) {
			f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------
