/******************************************************************************\
|                            pitaya_interface.cpp                              |
\******************************************************************************/

#include <mutex>


#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h> 

#include <unistd.h>

#include <arpa/inet.h>

#include "rapidjson/document.h"     // rapidjson's DOM-style API
#include "rapidjson/prettywriter.h" // for stringify JSON
#include "redpitaya/include/rp.h"

#include "pitaya_interface.h"

#include "rsa.h"
#include "misc.h"
#include "const_strings.h"
#include "rp_signal.h"

using namespace rapidjson;
using namespace std;
//-----------------------------------------------------------------------------
std::mutex mutexSender;
static bool s_fStopSender=false;
extern std::mutex mutexQueue;
extern queue<TRpSignal> qSignals;
//-----------------------------------------------------------------------------
int GetSample (float *afSamples, int nSampleLength)
{
	TRpSignal sig;

	if (qSignals.size() > 0) {
		mutexQueue.lock();
		sig = qSignals.front();
		qSignals.pop();
		mutexQueue.unlock();
	}
	int n = sig.GetSamplesToBuffer(afSamples, nSampleLength);
	return (n);
}
//-----------------------------------------------------------------------------
bool SendSamples (float *afSamples, int nSampleLength, const std::string &strClientIP, int nPort)
{
	int socketReturn, valread;
	struct sockaddr_in serv_addr;
	int addrlen = sizeof(serv_addr);
	char szBuffer[1024];

	if ((socketReturn = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		return (false);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(nPort);
	if(inet_pton(AF_INET, strClientIP.c_str(), &serv_addr.sin_addr) <=0 )
		return (false);
	if (connect(socketReturn, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
		return (false);
	send (socketReturn, afSamples, nSampleLength * sizeof (afSamples[0]), 0);
	valread = read( socketReturn, szBuffer, 1024);
	return (valread > 0);
}
//-----------------------------------------------------------------------------
void SetQuitSender (bool f)
{
	mutexSender.lock();
	s_fStopSender = f;
	mutexSender.unlock();
}
//-----------------------------------------------------------------------------

void BinSender (TPitayaInterface *pInterface)
{
	bool fQuit = false;
	
	while (!fQuit) {
		std::this_thread::sleep_for(2000ms);
		float *afSamples = new float [pInterface->GetSampleLength()];
		if (GetSample (afSamples, pInterface->GetSampleLength()) > 0)
			if (!SendSamples (afSamples, pInterface->GetSampleLength(), pInterface->GetClientIP(), pInterface->GetReturnPort()))
				fQuit = true;
		delete[] afSamples;
		if (!fQuit) {
			mutexSender.lock();
			fQuit = s_fStopSender;
			mutexSender.unlock();
		}
	}
}
//-----------------------------------------------------------------------------

TPitayaInterface::TPitayaInterface ()
{
	SetDefaultParams ();
	m_pSendThread = NULL;
}
//-----------------------------------------------------------------------------

TPitayaInterface::TPitayaInterface (const char *szClientIP)
{
	m_strClient = std::string(szClientIP);
	//m_document.CopyFrom (document, a);
	SetDefaultParams ();
	m_pSendThread = NULL;
}
//-----------------------------------------------------------------------------

TPitayaInterface::~TPitayaInterface ()
{
	if (m_pSendThread != NULL) {
		mutexSender.lock();
		s_fStopSender = true;
		mutexSender.unlock();
		m_pSendThread->join();
		delete m_pSendThread;
	}
}
//-----------------------------------------------------------------------------

void TPitayaInterface::SetDefaultParams ()
{
	m_nSamples   = 1000;
	m_nSampleLen = 6250;
	m_nReturnPort = 5501;
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::FollowCommand (const Document &docCommand, std::string &strReply)
{
	string strCommand;
	bool f = false;
	
	try {
		if (docCommand.HasMember (RPSU_SET_TRIGGER)) {
			const Value &valTrigger = docCommand[RPSU_SET_TRIGGER];
			if ((f = SetTrigger(valTrigger, strReply)) == true)
				f = GetTriggerString (strReply);
		}
		if (docCommand.HasMember (RPSU_GET_TRIGGER)) {
			f = GetTriggerString (strReply);
		}
		if (docCommand.HasMember (RPSU_SAMPLING)) {
			f = HandleSampling (docCommand[RPSU_SAMPLING], strReply);
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
		Value &valTrigger = docSetup[RPSU_SET_TRIGGER];
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
		Value &valTrigger = docSetup[RPSU_SET_TRIGGER];
		Value &valLevel = valTrigger[szTriggerItem];
		valLevel.SetString (str.c_str(), str.size());
		fUpdate = WriteJson (docSetup);
	}
	return (fUpdate);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::GetTriggerString (std::string &strReply)
{
	Document docSetup;
	bool fRead = false;

	if (ReadPitayaSetup (docSetup)) {
		Value &valTrigger = docSetup[RPSU_SET_TRIGGER];
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		valTrigger.Accept(writer);
		const char* szJson = buffer.GetString();
		strReply = std::string(szJson);
		fRead = true;
	}
	return (fRead);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::HandleSampling (const Value &valSampling, std::string &strReply)
{
	bool f = true;
	const char *szCommand;

	if (valSampling.HasMember (RPSU_OUTPUT))
		f = HandleSamplingParams (valSampling[RPSU_OUTPUT], strReply);
	if (valSampling.HasMember (RPSU_RET_PORT))
		m_nReturnPort = valSampling[RPSU_RET_PORT].GetInt();
	if (valSampling.HasMember (RPSU_COMMAND)) {
		szCommand = valSampling[RPSU_COMMAND].GetString();
		if (strcmp(szCommand, RPSU_START) == 0) {
			if (m_pSendThread == NULL) {
				SetQuitSender (false);
				m_pSendThread = new thread (BinSender, this);
			}
		}
		else if ((strcmp(szCommand, RPSU_STOP)) == 0) {
			SetQuitSender (true);
			m_pSendThread->join();
			delete m_pSendThread;
			m_pSendThread = NULL;
		}
	}
	return (f);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::HandleSamplingParams (const Value &valParams, std::string &strReply)
{
	bool f = true;

	try {
		if (valParams.HasMember (RPSU_SAMPLES))
			m_nSamples = valParams[RPSU_SAMPLES].GetInt();
		if (valParams.HasMember (RPSU_SAMPLE_LEN))
			m_nSampleLen = valParams[RPSU_SAMPLE_LEN].GetInt();
		f = true;
	}
	catch (std::exception &e) {
		fprintf (stderr, "Runtime error:\n%s\n", e.what());
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

std::string TPitayaInterface::GetClientIP() const
{
	return (m_strClient);
}
//-----------------------------------------------------------------------------

int TPitayaInterface::GetSamplesCount() const
{
	return (m_nSamples);
}
//-----------------------------------------------------------------------------

int TPitayaInterface::GetSampleLength() const
{
	return (m_nSampleLen);
}
//-----------------------------------------------------------------------------

int TPitayaInterface::GetReturnPort() const
{
	return (m_nReturnPort);
}
//-----------------------------------------------------------------------------
