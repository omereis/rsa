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

#include "redpitaya/include/rp.h"

#include "jsoncpp/json/json.h"

#include "pitaya_interface.h"
#include "rsa.h"
#include "misc.h"
#include "const_strings.h"
#include "rp_signal.h"

using namespace std;
//-----------------------------------------------------------------------------
std::mutex mutexSender;

std::mutex mutexSampling;

static bool s_fStartSampling;
static bool s_fStopSender=false;
extern std::mutex mutexQueue;
static queue<TRpSignal> qSignals;
//-----------------------------------------------------------------------------
void SetQuitSender (bool f)
{
	mutexSender.lock();
	s_fStopSender = f;
	if (s_fStopSender)
		s_fStopSender = true;
	mutexSender.unlock();
}
//-----------------------------------------------------------------------------

void SetStartSampling (bool f)
{
	mutexSampling.lock();
	s_fStartSampling = f;
	mutexSampling.unlock();
}
//-----------------------------------------------------------------------------

bool GetStartSampling ()
{
	bool f;
	
	mutexSampling.lock();
	f = s_fStartSampling;
	mutexSampling.unlock();
	return (f);
}

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

void BinSender (TPitayaInterface *pInterface)
{
	bool fQuit = false;
	int nSamplesSend=0;
	
	while (!fQuit) {
		float *afSamples = new float [pInterface->GetSampleLength()];
		if (GetSample (afSamples, pInterface->GetSampleLength()) > 0)
			if (!SendSamples (afSamples, pInterface->GetSampleLength(), pInterface->GetClientIP(), pInterface->GetReturnPort()))
				fQuit = true;
			else {
				if (pInterface->GetSamplesCount() > 0) {
					nSamplesSend++;
					if (nSamplesSend >= pInterface->GetSamplesCount())
						fQuit = true;
				}
			}
		delete[] afSamples;
		std::this_thread::sleep_for(2000ms);
		if (!fQuit) {
			mutexSender.lock();
			fQuit = s_fStopSender;
			mutexSender.unlock();
		}
		else
			nSamplesSend += 0;
	}
	nSamplesSend += 0;
}
//-----------------------------------------------------------------------------

/******************************************************************************\
|                              Sampler Thread                                  |
\******************************************************************************/
bool read_fast_analog (float *buff, uint32_t buff_size);
//-----------------------------------------------------------------------------

/*
Sampler Pseudo Code
-------------------
	WaitForStart(timeout)
	while (!QuitThread) {
		GetSample (signal, timeout)
		if (!timeout) {
			queue.push(signal)
			if (psd_required) {
				psd_results = psd_analysis(signal)
				qPsd.push (psd_results)
			}
			if (mca_required) {
				mca_results = max_analysis (signal)
				qMca.push (mca_results)
			}
		}
		SamplerCommand = GetSamplingCommand()
		if (SamplerCommand == StopSampling)
			WaitForStart(timeout)
		else if (SamplerCommand == QuiteThread)
			QuiteThread = true;
	}
*/	

//-----------------------------------------------------------------------------

/******************************************************************************\
|                        TPitayaInterface class                                |
\******************************************************************************/
TPitayaInterface::TPitayaInterface ()
{
	m_rp_params.Clear ();
	SetDefaultParams ();
	m_pSendThread = NULL;
	m_pSampler = NULL;
}
//-----------------------------------------------------------------------------

TPitayaInterface::TPitayaInterface (const char *szClientIP)
{
	m_strClient = std::string(szClientIP);
	SetDefaultParams ();
	m_pSendThread = NULL;
	m_pSampler = NULL;
}
//-----------------------------------------------------------------------------

TPitayaInterface::~TPitayaInterface ()
{
	if (m_pSendThread != NULL) {
		SetQuitSender (true);
		m_pSendThread->join();
		delete m_pSendThread;
	}
	if (m_pSampler != NULL) {
		m_pSampler->join();
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

bool TPitayaInterface::FollowCommand (Json::Value &root, std::string &strReply)
{
	string strCommand;
	bool f = false;
	
	try {
		strReply = StringifyJson (root);
		strReply = "";
		//if (!root[RPSU_SET_TRIGGER].isNull())
			//f = m_rp_params.SetTrigger (root[RPSU_SET_TRIGGER], strReply);
		if (!root[RPSU_SETUP].isNull())
			f = HandleSetupCommand (root[RPSU_SETUP], strReply);
		else if (!root[RPSU_SAMPLING].isNull())
			f = HandleSampling (root[RPSU_SAMPLING], strReply);
			//f = m_rp_params.SetSamplingParams (root[RPSU_SAMPLING], strReply);
	}
	catch (exception &e) {
		strReply = e.what();
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::HandleSetupCommand (Json::Value &root, std::string &strReply)
{
	bool f;
	std::string strCommand, str;

	if (root.isString()) {
		strCommand = ToLower (root.asString());
		if (strCommand == std::string(RPSU_GET_SETUP))
			f = m_rp_params.GetSamplingParams (strReply);
		if (strCommand == std::string(RPSU_GET_TRIGGER)) {
			if ((f = m_rp_params.GetTriggerAsString (str)) == true)
				strReply += "\n" + str;
		}
	}
	if (root.isObject()) {
	}
	else
		f = true;
/*
	if (!root[RPSU_GET_SETUP].isNull())
		f = m_rp_params.GetSamplingParams (strReply);
	if (!root[RPSU_GET_TRIGGER].isNull())
		f = GetTriggerString (strReply);
*/
	return (f);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::HandleSampling (const Json::Value &root, std::string &strReply)
{
	bool f;
	if (root.isObject()) {
		if (!root[RPSU_SAMPLING].isNull()) {
			std::string strCommand = root[RPSU_SAMPLING].asString();
			if (strCommand == std::string (RPSU_START))
				f = StartSampling (strReply);
			else if (strCommand == std::string (RPSU_STOP))
				f = StopSampling (strReply);
		}
	}
	else
		f = true;
	return (f);
}
#include <condition_variable>
#include <cstddef>
#include <iostream>

//-----------------------------------------------------------------------------
std::condition_variable cond;
std::mutex mtx;

static bool s_fQuitSamplingThread;
std::mutex mtxQuitThread;

bool GetQuitSamplingThread ()
{
	bool f;

	mtxQuitThread.lock ();
	f = s_fQuitSamplingThread;
	mtxQuitThread.unlock ();
	return (f);
}
//-----------------------------------------------------------------------------
void SetQuitSamplingThread (bool f)
{
	mtxQuitThread.lock ();
	s_fQuitSamplingThread = f;
	mtxQuitThread.unlock ();
}
//-----------------------------------------------------------------------------
void WaitForStart(int timeout)
{
	bool fDoneWaiting=false;

    std::unique_lock<std::mutex> lck(mtx);
	while ((fDoneWaiting == false) && (cond.wait_for(lck,std::chrono::milliseconds(timeout))==std::cv_status::timeout)) {
		fDoneWaiting = GetStartSampling ();
	}
	//cond.wait(lck, GetStartSampling);
}
using namespace std::chrono_literals;
//-----------------------------------------------------------------------------
void Sampler (TPitayaInterface *pInterface)
{
	bool fQuitSamplingThread = false;

	fprintf (stderr, "Sampler started\n");
	WaitForStart(500);
	while (!fQuitSamplingThread) {
		std::this_thread::sleep_for(500ms);
		fQuitSamplingThread = GetQuitSamplingThread ();
	}
	fprintf (stderr, "Sampler Ended\n");
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::StartSampling (std::string &strReply)
{
	bool f;
	
	try {
		SetQuitSamplingThread (false);
		if (m_pSampler == NULL)
			m_pSampler = new thread(Sampler, this);
		SetStartSampling (true);
		strReply = "Sampling Started";
		f = true;
		fprintf (stderr, "\nSampling Started\n");
	}
	catch (std::exception &e) {
		strReply = std::string("Runtime error in 'TPitayaInterface::StartSampling'\n") + e.what();
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::StopSampling (std::string &strReply)
{
	bool f;
	
	try {
		SetQuitSamplingThread (true);
		SetStartSampling (true);
		if (m_pSampler != NULL) {
			m_pSampler->join();
			m_pSampler = NULL;
		}
		strReply = "Sampling Ended";
		f = true;
		fprintf (stderr, "\nSampling Ended\n");
	}
	catch (std::exception &e) {
		strReply = std::string("Runtime error in 'TPitayaInterface::StopSampling'\n") + e.what();
		f = false;
	}
	return (f);
}
//-----------------------------------------------------------------------------

bool TPitayaInterface::GetTriggerString (std::string &strReply)
{
	Json::Value jvalTrigger, valRedPitayaSetup;
	bool fRead = false;

	try {
		m_rp_params.GetTrigger (jvalTrigger);
		strReply = StringifyJson (jvalTrigger);
		fRead = true;
	}
	catch (std::exception &e) {
		fprintf (stderr, "Runtime error in GetTriggerString:\n%s\n", e.what());
		fRead = false;
	}
	return (fRead);
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

void TPitayaInterface::SetClientIP (const char *szClientIP)
{
	m_strClient = std::string(szClientIP);
}
//-----------------------------------------------------------------------------

void TPitayaInterface::LoadSetup()
{
	m_rp_params.LoadFromJson (PITAYA_MOCK_NAME);
}
//-----------------------------------------------------------------------------
bool TPitayaInterface::SetSamplingParams (Json::Value &jSampling, std::string &strReply)
{
}
//-----------------------------------------------------------------------------
