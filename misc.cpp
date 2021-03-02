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

std::string TriggerSourceName (const rp_acq_trig_src_t &m_source)
{
	std::string strName;
	
    if (m_source == RP_TRIG_SRC_DISABLED)
		strName = "disabled";
    else if (m_source == RP_TRIG_SRC_NOW)
		strName = "now";
    else if (m_source == RP_TRIG_SRC_CHA_PE)
		strName = "in1";
    else if (m_source == RP_TRIG_SRC_CHA_NE)
		strName = "in1";
    else if (m_source == RP_TRIG_SRC_CHB_PE)
		strName = "in2";
    else if (m_source == RP_TRIG_SRC_CHB_NE)
		strName = "in2";
    else if (m_source == RP_TRIG_SRC_EXT_PE)
		strName = "external";
    else if (m_source == RP_TRIG_SRC_EXT_NE)
		strName = "external";
    else if (m_source == RP_TRIG_SRC_AWG_PE)
		strName = "generator";
    else if (m_source == RP_TRIG_SRC_AWG_NE)
		strName = "generator";
	else
		strName = "unknown";
	return (strName);
}
//-----------------------------------------------------------------------------

std::string TriggerDirName (const rp_acq_trig_src_t &m_source)
{
	std::string strName;
	
    if (m_source == RP_TRIG_SRC_DISABLED)
		strName = "disabled";
    else if (m_source == RP_TRIG_SRC_NOW)
		strName = "now";
    else if (m_source == RP_TRIG_SRC_CHA_PE)
		strName = "rise";
    else if (m_source == RP_TRIG_SRC_CHA_NE)
		strName = "fall";
    else if (m_source == RP_TRIG_SRC_CHB_PE)
		strName = "rise";
    else if (m_source == RP_TRIG_SRC_CHB_NE)
		strName = "fall";
    else if (m_source == RP_TRIG_SRC_EXT_PE)
		strName = "rise";
    else if (m_source == RP_TRIG_SRC_EXT_NE)
		strName = "fall";
    else if (m_source == RP_TRIG_SRC_AWG_PE)
		strName = "rise";
    else if (m_source == RP_TRIG_SRC_AWG_NE)
		strName = "fall";
	else
		strName = "unknown";
	return (strName);
}
//-----------------------------------------------------------------------------

bool SetTriggerFromName (const std::string &strSourceIn, const std::string &strSourceDir, rp_acq_trig_src_t & m_source)
{
	std::string strIn = ToLower(strSourceIn);
	std::string strDir = ToLower (strSourceDir);
	bool fAssign = true;
	
	if (strIn == "in1") {
		if (strDir == "rise")
			m_source = RP_TRIG_SRC_CHA_PE;
		else if (strDir == "fall")
			m_source = RP_TRIG_SRC_CHA_NE;
		else
			fAssign = false;
	}
	else if (strIn == "in2") {
		if (strDir == "rise")
			m_source = RP_TRIG_SRC_CHB_PE;
		else if (strDir == "fall")
			m_source = RP_TRIG_SRC_CHB_NE;
		else
			fAssign = false;
	}
	else if (strIn == "now")
		m_source = RP_TRIG_SRC_NOW;
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
