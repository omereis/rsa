#include <stdio.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <string>
#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

std::mutex g_mtxTiming;

bool g_fTimeout;
bool g_fAddressFound;
string g_strAddress;
string g_strHostName;
//-----------------------------------------------------------------------------
void Timing ()
{
	g_mtxTiming.lock();
	g_fTimeout = false;
	sleep (1000);
	g_fTimeout = true;
	g_mtxTiming.unlock();
}
//-----------------------------------------------------------------------------
void FindIP ()
{
	g_fAddressFound = false;
	hostent* hostname = gethostbyname(g_strHostName.c_str());
	if(hostname)
		g_strAddress = std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
	g_fAddressFound = true;
}
//-----------------------------------------------------------------------------
std::string FindHostIPByName (const std::string &strHostName)
{
	hostent* hostname = gethostbyname(strHostName.c_str());
	std::string strAddress;

	if(hostname)
		strAddress = std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
	return (strAddress);
}
//-----------------------------------------------------------------------------
std::string FindHostIP (const std::string &strHost, int nTimeout)
{
	g_strAddress = "";
	g_strHostName = strHost;
	g_fTimeout = g_fAddressFound = false;
	std::thread thTimeout (Timing);
	std::thread thFind (FindIP);
	while (g_fTimeout || g_fAddressFound)
		sleep (10);
	return (g_strHostName);
}
//-----------------------------------------------------------------------------
std::string HostToIp(const std::string& host)
{
	g_strAddress = "";
	g_fAddressFound = false;
	hostent* hostname = gethostbyname(host.c_str());
	if(hostname)
		g_strAddress = std::string(inet_ntoa(**(in_addr**)hostname->h_addr_list));
	g_fAddressFound = true;
	return (g_strAddress);
}
//-----------------------------------------------------------------------------
int main (int argc, char *argv[])
{
	string str;
	bool f;
	struct hostent *h;

	printf ("Testing 'gethostbyname'\n");
	do {
		printf ("Please enter host name...");
		getline (cin, str);
		f = (str.size() > 0);
		if (f) {
			cout << "Required host: '" << str << "'\n";
			//h = gethostbyname (str.c_str());
			fprintf (stderr, "Testing FindHostIPByName : address of '%s' it '%s'\n", str.c_str(), FindHostIPByName (str).c_str());
			//cout << "FindHostIP result:\n'" << FindHostIP (str, 1000);
			//cout << "HostToIP result:\n'" << HostToIp (str) << "'\n";
			f = true; // provide breakpoint
		}
	} while (f);
	cout << "Bye\n";
}
//-----------------------------------------------------------------------------
