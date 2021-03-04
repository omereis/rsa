/******************************************************************************\
|                                  rp_params.h                                 |
\******************************************************************************/

#ifndef	__RP_PARAMS_H
#define	__RP_PARAMS_H

#include <string>
#include "misc.h"
#include "rp_trigger.h"
#include "jsoncpp/json/json.h"

using namespace std;

class TRedPitayaParams {
public:
	TRedPitayaParams ();

	bool Clear ();
	bool LoadFromJson (const std::string &strJson);

	void GetTrigger (Json::Value &jvalSetup);
	bool SetTrigger (const Json::Value &jsonTrigger, std::string &strReply);
	rp_acq_sampling_rate_t GetRate() const;
	void SetRate (rp_acq_sampling_rate_t rate);
	rp_channel_t GetChannel () const;
	void SetChannel (rp_channel_t channel);
	rp_acq_decimation_t GetDecimation () const;
	void GetDecimation (rp_acq_decimation_t decimation);

	string GetRateName () const;
	void SetRate (const string &strRate);
	string GetChannelName () const;
	void SetChannel (const string &strChannel);

	bool GetSamplingParams (std::string &strReply);
	bool SetSamplingParams (const Json::Value &jParams, std::string &strReply);

	bool GetTriggerAsString (std::string &strReply) const;
protected:
	bool GetParams (Json::Value &jParams) const;
private:
	TRedPitayaTrigger m_trigger;
	rp_acq_sampling_rate_t m_rate;
	rp_channel_t m_channel;
	rp_acq_decimation_t m_decimation;
};
#endif // __RP_PARAMS_H
