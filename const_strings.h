#ifndef	__CONST_STRINGS_H_INC
#define	__CONST_STRINGS_H_INC

#include <string>

using namespace std;

extern const std::string g_strTrigger;
extern const std::string g_strLevel;

#define	RPSU_SET_TRIGGER	"set_trigger" // Red Pitaya Set Up
#define	RPSU_GET_TRIGGER	"get_trigger" // Red Pitaya Set Up
#define	RPSU_LEVEL		"level"
#define	RPSU_DELAY		"delay"
#define	RPSU_SOURCE		"source"
#define	RPSU_DIR		"direction"
#define	RPSU_SAMPLING	"sampling"
#define	RPSU_COMMAND	"command"
#define	RPSU_START		"start"
#define	RPSU_STOP		"stop"
#define	RPSU_OUTPUT		"output"
#define	RPSU_SAMPLES	"samples"
#define	RPSU_SAMPLE_LEN	"sample_length" 
#define	RPSU_RET_PORT	"return_port"


//-----------------------------------------------------------------------------
#endif //__CONST_STRINGS
