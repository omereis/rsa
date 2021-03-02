/******************************************************************************\
|                                  rp_params.h                                 |
\******************************************************************************/

#ifndef	__RP_PARAMS_H
#define	__RP_PARAMS_H

#include "misc.h"
#include "rp_trigger.h"
#include "jsoncpp/json/json.h"

class TRedPitayaParams {
public:
	TRedPitayaParams ();

	bool Clear ();
	bool LoadFromJson (const std::string &strJson);

	//bool GetTrigger (Document &docTrigger);
	//bool GetSetup (const Value &valSetup, std::string &strReply);
	void GetTrigger (Json::Value &jvalSetup);
	bool SetTrigger (const Json::Value &jsonTrigger, std::string &strReply);
protected:
private:
	TRedPitayaTrigger m_trigger;
};
#endif // __RP_PARAMS_H
