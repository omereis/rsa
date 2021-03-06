#ifndef	_RSA_INC
#define	_RSA_INC

#include <queue>
#include <vector>
#include <string>

#include "jsoncpp/json/json.h"
using namespace std;

#define	DEFAULT_PORT	5500
#define	DEFAULT_ADDRESS	"127.0.0.1"

#define	COMM_BUFFER_LENGTH	8192

#define	MAX_BUFF_SIZE	8192

#define	MAX_QUEUE_LENGTH	1000

#ifndef	PITAYA_MOCK_NAME
#define	PITAYA_MOCK_NAME	"red_pitaya.json"
#endif	//PITAYA_MOCK_NAME

//-----------------------------------------------------------------------------
#ifndef TStringQueue
typedef queue<string> TStringQueue;
#endif // TStringQueue
//-----------------------------------------------------------------------------
#ifndef TStringVector
typedef vector<string> TStringVector;
#endif // TStringVector
//-----------------------------------------------------------------------------
#ifndef TFloatVector
typedef vector<float> TFloatVector;
#endif // TStringVector
//-----------------------------------------------------------------------------

enum ERPCommand {ERPC_SAMPLING, ERPC_GET_TRIGGER, ERPC_SET_TRIGGER, ERPC_GET_SIGNALS, ERPC_GET_SAMPLES, ERPC_NONE};

#endif // RSA
