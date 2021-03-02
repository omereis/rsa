/******************************************************************************\
|                                  rp_trigger.h                                |
\******************************************************************************/
#ifndef	__RP_TRIGGER_H
#define	__RP_TRIGGER_H

#include <string>
#include "rp.h"
#include "misc.h"
#include "jsoncpp/json/json.h"

class TRedPitayaTrigger {
public:
	TRedPitayaTrigger ();
	TRedPitayaTrigger (const TRedPitayaTrigger &other);
	TRedPitayaTrigger operator= (const TRedPitayaTrigger &other);
	bool operator== (const TRedPitayaTrigger &other) const;
	bool operator!= (const TRedPitayaTrigger &other) const;
	void Clear ();
	//bool LoadFromJson (const Value &valTrigger);
	bool LoadFromJson (const std::string &strFile);
	bool SaveToJson  (const std::string &strFile);
	//bool  JsonTrigger (Document &docTrigger);
	void GetJson (Json::Value &valTrigger);

	bool LoadFromCard ();
	bool UpdateInCard ();
	
	float GetLevel () const;
	void SetLevel (float fLevel);
	float GetDelay ()const;
	void SetDelay (float fDelay);
	std::string GetInput () const;
	bool SetInput (const std::string &strIn);
	std::string GetDirection () const;
	bool SetDirection (const std::string strDir);
protected:
	void AssignAll (const TRedPitayaTrigger &other);
private:
	float m_fLevel;
	float m_fDelay;
	std::string m_strDir;
	std::string m_strSource;
	rp_channel_t m_channel;
	rp_acq_trig_src_t m_source;
};
#endif // __RP_TRIGGER_H
