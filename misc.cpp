/*
m i s c . c p p
*/


#include "misc.h"
#include "rsa.h"
#include <stdio.h>

#include "rapidjson/filewritestream.h"
#include <rapidjson/writer.h>
#include <cstdio>

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

bool ReadPitayaSetup (Document &docSetup)
{
	std::string str;
	bool fRead;

	try {
		if ((fRead = ReadFile (PITAYA_MOCK_NAME, str)) == true) {
			docSetup.Parse(str.c_str());
			fRead != docSetup.HasParseError();
		}
	}
	catch (std::exception &e){
		fRead = false;
	}
	return (fRead);
}
//-----------------------------------------------------------------------------

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
//-----------------------------------------------------------------------------
