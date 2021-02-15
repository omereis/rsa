#ifndef	_RSA_INC
#define	_RSA_INC

#include <queue>
#include <vector>
#include <string>

using namespace std;

//-----------------------------------------------------------------------------
#ifndef TStringQueue
typedef queue<string> TStringQueue;
#endif // TStringQueue
//-----------------------------------------------------------------------------
#ifndef TStringVector
typedef vector<string> TStringVector;
#endif // TStringVector
//-----------------------------------------------------------------------------

enum ERPCommand {ERPC_SAMPLING, ERPC_ERPC_GET_SETUP, ERPC_SET_TRIGGER, ERPC_GET_SIGNALS, ERPC_GET_SAMPLES, ERPC_NONE};

#endif // _RSA_INC
