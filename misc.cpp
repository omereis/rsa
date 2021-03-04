/*
m i s c . c p p
*/


#include "misc.h"
#include "rsa.h"
#include <stdio.h>

//#include "rapidjson/filewritestream.h"
//#include <rapidjson/writer.h>
//#include <cstdio>

//using namespace rapidjson;
using namespace std;

//-----------------------------------------------------------------------------
bool ReadFile (const std::string &strFileName, std::string &strContent)
{
	bool fRead;
	std::string str;

	try {
		strContent = "";
		FILE *file;
		if ((file = fopen (strFileName.c_str(), "r")) != NULL) {
			char *szBuf = new char[4096];
			while (fgets(szBuf, 4000, file) != NULL)
				str += std::string(szBuf);
			fclose (file);
			delete[] szBuf;
			strContent = str;
		}
		fRead = true;
	}
	catch (std::exception &e) {
		fRead = false;
	}
	return (fRead);
}
//-----------------------------------------------------------------------------

bool ReadPitayaSetup (Json::Value &valSetup)
{
	std::string str (PITAYA_MOCK_NAME);

	return (ReadPitayaSetup (str, valSetup));
}
//-----------------------------------------------------------------------------

/*
bool ReadPitayaSetup (Document &docSetup)
{
	std::string str (PITAYA_MOCK_NAME);

	return (ReadPitayaSetup (str, docSetup));
}
*/
//-----------------------------------------------------------------------------

bool ReadPitayaSetup (std::string &strJsonFile, Json::Value &valSetup)
{
	std::string str;
	Json::Reader reader;
	bool fRead;

	try {
		if ((fRead = ReadFile (strJsonFile, str)) == true) {
			fRead = reader.parse(str, valSetup);
		}
	}
	catch (std::exception &e){
		fRead = false;
	}
	return (fRead);
}
//-----------------------------------------------------------------------------

/*
bool ReadPitayaSetup (std::string &strJson, Document &docSetup)
{
	std::string str;
	bool fRead;

	try {
		if ((fRead = ReadFile (strJson, str)) == true) {
			docSetup.Parse(str.c_str());
			fRead = !docSetup.HasParseError();
		}
	}
	catch (std::exception &e){
		fRead = false;
	}
	return (fRead);
}
*/
//-----------------------------------------------------------------------------

/*
bool WriteJson (Document &doc)
{
	// example from https://rapidjson.org/md_doc_stream.html
	FILE *file;
	bool fWrite;
	std::string strFileName (PITAYA_MOCK_NAME);

	if ((file = fopen (strFileName.c_str(), "wb")) != NULL) {
		char *szWriteBuffer = new char [8192];
		FileWriteStream os(file, szWriteBuffer, sizeof(szWriteBuffer));
		Writer<FileWriteStream> writer(os);
		doc.Accept(writer);
		fclose (file);
		fWrite = true;
	}
	else
		fWrite = false;
	return (fWrite);
}
*/
//-----------------------------------------------------------------------------

int OpenServerSocket (int nPort)
{
	struct sockaddr_in address; 
	int opt = 1; 
	int nServerFd;

	// Creating socket file descriptor 
	if ((nServerFd = socket(AF_INET, SOCK_STREAM, 0)) == 0) { 
		perror("socket failed"); 
		exit(EXIT_FAILURE); 
	} 

	// Forcefully attaching socket to the port
	if (setsockopt(nServerFd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))  { 
		perror("setsockopt"); 
		exit(EXIT_FAILURE); 
	} 
	address.sin_family = AF_INET; 
	address.sin_addr.s_addr = INADDR_ANY; 
	address.sin_port = htons(nPort); 

	// Forcefully attaching socket to the port
	if (bind(nServerFd, (struct sockaddr *)&address, sizeof(address)) < 0) { 
		perror("bind failed"); 
		exit(EXIT_FAILURE); 
	} 
	if (listen(nServerFd, 3) < 0)  { 
		perror("listen"); 
		exit(EXIT_FAILURE); 
	}
	return (nServerFd);
}
//-----------------------------------------------------------------------------
#include <string.h>
#include <strings.h>

bool IsJsonQuit (const char *szJson)
{
	bool fQuit=false;
	Json::Value vJson;
	Json::Reader reader;
	std::string strJson = ToLower (std::string(szJson));

	if (reader.parse(strJson.c_str(), vJson)) {
		if (!vJson["quit"].isNull())
			if (vJson["quit"].asString() == std::string("quit"))
				fQuit = true;
	}
	return (fQuit);
}
//-----------------------------------------------------------------------------

string ToLower (const std::string &str)
{
	string strLower;

	for (int n=0 ; n < str.size() ; n++)
		strLower += tolower(str[n]);
	return (strLower);
}
//-----------------------------------------------------------------------------

std::string TriggerSourceName (const rp_acq_trig_src_t &source)
{
	std::string strName;
	
    if (source == RP_TRIG_SRC_DISABLED)
		strName = "disabled";
    else if (source == RP_TRIG_SRC_NOW)
		strName = "now";
    else if (source == RP_TRIG_SRC_CHA_PE)
		strName = "in1";
    else if (source == RP_TRIG_SRC_CHA_NE)
		strName = "in1";
    else if (source == RP_TRIG_SRC_CHB_PE)
		strName = "in2";
    else if (source == RP_TRIG_SRC_CHB_NE)
		strName = "in2";
    else if (source == RP_TRIG_SRC_EXT_PE)
		strName = "external";
    else if (source == RP_TRIG_SRC_EXT_NE)
		strName = "external";
    else if (source == RP_TRIG_SRC_AWG_PE)
		strName = "generator";
    else if (source == RP_TRIG_SRC_AWG_NE)
		strName = "generator";
	else
		strName = "unknown";
	return (strName);
}
//-----------------------------------------------------------------------------

std::string TriggerDirName (const rp_acq_trig_src_t &source)
{
	std::string strName;
	
    if (source == RP_TRIG_SRC_DISABLED)
		strName = "disabled";
    else if (source == RP_TRIG_SRC_NOW)
		strName = "now";
    else if (source == RP_TRIG_SRC_CHA_PE)
		strName = "rise";
    else if (source == RP_TRIG_SRC_CHA_NE)
		strName = "fall";
    else if (source == RP_TRIG_SRC_CHB_PE)
		strName = "rise";
    else if (source == RP_TRIG_SRC_CHB_NE)
		strName = "fall";
    else if (source == RP_TRIG_SRC_EXT_PE)
		strName = "rise";
    else if (source == RP_TRIG_SRC_EXT_NE)
		strName = "fall";
    else if (source == RP_TRIG_SRC_AWG_PE)
		strName = "rise";
    else if (source == RP_TRIG_SRC_AWG_NE)
		strName = "fall";
	else
		strName = "unknown";
	return (strName);
}
//-----------------------------------------------------------------------------

bool SetTriggerFromName (const std::string &strSourceIn, const std::string &strSourceDir, rp_acq_trig_src_t & source)
{
	std::string strIn = ToLower(strSourceIn);
	std::string strDir = ToLower (strSourceDir);
	bool fAssign = true;
	
	if (strIn == "in1") {
		if (strDir == "rise")
			source = RP_TRIG_SRC_CHA_PE;
		else if (strDir == "fall")
			source = RP_TRIG_SRC_CHA_NE;
		else
			fAssign = false;
	}
	else if (strIn == "in2") {
		if (strDir == "rise")
			source = RP_TRIG_SRC_CHB_PE;
		else if (strDir == "fall")
			source = RP_TRIG_SRC_CHB_NE;
		else
			fAssign = false;
	}
	else if (strIn == "now")
		source = RP_TRIG_SRC_NOW;
	else
		fAssign = false;
	return (fAssign);
}
//-----------------------------------------------------------------------------

// source: http://www.martinbroadhurst.com/how-to-trim-a-stdstring.html
std::string& ltrim(std::string& str, const std::string& chars)
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}
//-----------------------------------------------------------------------------
 
std::string& rtrim(std::string& str, const std::string& chars)
{
    str.erase(str.find_last_not_of(chars) + 1);
    return str;
}
//-----------------------------------------------------------------------------

std::string& trim(std::string& str, const std::string& chars)
{
    return ltrim(rtrim(str, chars), chars);
}
//-----------------------------------------------------------------------------

bool GetJsonInt (const Json::Value &val, const std::string &strKey, int &nValue)
{
	const char *szKey = strKey.c_str();
	bool fKeyExists = true;

	if (!val[szKey].isNull())
		if (val[szKey].isInt())
			nValue = val[szKey].asInt();
		else if (val[szKey].isString()) {
			//const char *sz = val[szKey].asString();
			std::string s = val[szKey].asString();
			nValue = atoi(s.c_str());
			//nValue = atoi(sz);
		}
		else
			fKeyExists = false;
	else
		fKeyExists = false;
	return (fKeyExists);
}
//-----------------------------------------------------------------------------

std::string StringifyJson (const Json::Value &val)
{
	std::string strJson;
	Json::FastWriter fastWriter;
	
	strJson = fastWriter.write(val);
	return (strJson);
}
//-----------------------------------------------------------------------------

std::string SamplingRateName (rp_acq_sampling_rate_t rate)
{
	std::string strRate;
    
	if (rate == RP_SMP_125M)
		strRate = "125Mhz";
   else if (rate == RP_SMP_15_625M)
		strRate = "15.625Mhz";
    else if (rate == RP_SMP_1_953M)
		strRate = "1.963Mhz";
    else if (rate == RP_SMP_122_070K)
		strRate = "122.070Khz";
    else if (rate == RP_SMP_15_258K)
		strRate = "15.258Khz";
    else if (rate == RP_SMP_1_907K)
		strRate = "1.907Khz";
	else
		strRate = "";
	return (strRate);
}
//-----------------------------------------------------------------------------
rp_acq_sampling_rate_t SamplingRateFromName (const std::string &str)
{
	std::string strRate = ToLower (str);
	rp_acq_sampling_rate_t rate;
    
	if (strRate == "125mhz")
		rate = RP_SMP_125M;
   else if (strRate == "15.625mhz")
	   rate = RP_SMP_15_625M;
	else if (strRate == "1.963mhz")
		rate = RP_SMP_1_953M;
	else if (strRate == "122.07kKhz")
		rate = RP_SMP_122_070K;
	else if (strRate == "15.258khz")
		rate = RP_SMP_15_258K;
	else if (strRate == "1.907Khz")
		rate = RP_SMP_1_907K;
	else
		rate = (rp_acq_sampling_rate_t) -1;
}
//-----------------------------------------------------------------------------

std::string GetChannelName (rp_channel_t channel)
{
	 std::string strChannel;

	if (channel == RP_CH_1)
		strChannel = "in1";
	else if (channel == RP_CH_2)
		strChannel = "in2";
	else
		strChannel = "";
	return (strChannel);
}
//-----------------------------------------------------------------------------

rp_channel_t SetChannelFromName (const string &str)
{
	rp_channel_t channel;
	std::string strChannel = ToLower (str);
	
	if (strChannel == "in1")
		channel = RP_CH_1;
	else if (strChannel == "in2")
		channel = RP_CH_2;
	else
		channel = (rp_channel_t) -1;
	return (channel);
}
//-----------------------------------------------------------------------------

string GetDecimationName (rp_acq_decimation_t decimation)
{
	std::string strDecimation;
	
    if (decimation == RP_DEC_1)
		strDecimation = "1";
    else if (decimation == RP_DEC_8)
		strDecimation = "8";
    else if (decimation == RP_DEC_64)
		strDecimation = "64";
    else if (decimation == RP_DEC_1024)
		strDecimation = "1024";
    else if (decimation == RP_DEC_8192)
		strDecimation = "8191";
    else if (decimation == RP_DEC_65536)
		strDecimation = "65536";
	else
		strDecimation = "unknown";
	return (strDecimation);
}
//-----------------------------------------------------------------------------

rp_acq_decimation_t DecimationFromName (int nDecimation)
{
	rp_acq_decimation_t decimation;
	std::string strDecimation;
	
	if (nDecimation == 1)
		decimation = RP_DEC_1;
    else if (nDecimation == 8)
		decimation = RP_DEC_8;
    else if (nDecimation == 64)
		decimation = RP_DEC_64;
    else if (nDecimation == 1024)
		decimation = RP_DEC_1024;
    else if (nDecimation == 8191)
		decimation = RP_DEC_8192;
    else if (nDecimation == 65536)
		decimation = RP_DEC_65536;
	else
		decimation = (rp_acq_decimation_t) -1;
	return (decimation);
}
//-----------------------------------------------------------------------------
rp_acq_decimation_t DecimationFromName (const string &str)
{
	//int n = std::atoi(str.c_str());
	//DecimationFromName (n);
	//return (n);
	return (DecimationFromName (std::atoi(str.c_str())));
}
//-----------------------------------------------------------------------------
