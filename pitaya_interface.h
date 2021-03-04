#ifndef	__RPCommand_H
#define	__RPCommand_H

#include <thread>
#include <string>

#include "jsoncpp/json/json.h"

#include "redpitaya/include/rp.h"

#include "rsa.h"
#include "rp_params.h"

using namespace std;
enum EValueType {EVT_NONE, EVT_STRING, EVT_REAL};
//-----------------------------------------------------------------------------
class TPitayaInterface { // Res Pitaya Commands
public:
	TPitayaInterface ();
	TPitayaInterface (const char *szClientIP);
	~TPitayaInterface ();
	bool FollowCommand (Json::Value &root, std::string &strReply);

	std::string GetClientIP() const;
	int GetSamplesCount() const;
	int GetSampleLength() const;
	int GetReturnPort() const;

	void LoadSetup();
	void SetClientIP (const char *szClientIP);
protected:
	bool HandleSetupCommand (Json::Value &root, std::string &strReply);
	bool HandleSampling (const Json::Value &root, std::string &strReply);
	bool StartSampling (std::string &strReply);
	bool StopSampling (std::string &strReply);

	bool SetTriggerLevel (const string &strLevel, string &strReply);
	bool UpdateTriggerItem (const char *szTriggerItem, const double &dValue);
	bool UpdateTriggerItem (const char *szTriggerItem, const std::string &strLevel);
	bool GetTriggerString (std::string &strReply);

	void  SetDefaultParams ();
	bool HandleSamplingParams (const Json::Value &valParams, std::string &strReply);

	bool SetSamplingParams (Json::Value &jSampling, std::string &strReply);
	bool GetSamplingParams (std::string &strReply);

private:
	//Document m_document;
	TRedPitayaParams m_rp_params;
	int m_nSamples;
	int m_nSampleLen;
	int m_nReturnPort;
	size_t m_szQueue;
	thread *m_pSendThread;
	thread *m_pSampler;
	std::string m_strClient;
	
};
//-----------------------------------------------------------------------------
#endif // __RPCommand_H
