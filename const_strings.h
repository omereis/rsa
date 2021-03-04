#ifndef	__CONST_STRINGS_H_INC
#define	__CONST_STRINGS_H_INC

#include <string>

using namespace std;

extern const std::string g_strTrigger;
extern const std::string g_strLevel;

#define	RPSU_SET_TRIGGER	"set_trigger" // Red Pitaya Set Up
#define	RPSU_GET_TRIGGER	"get_trigger" // Red Pitaya Set Up
#define	RPSU_GET_SETUP		"get_setup" // Red Pitaya Set Up
#define	RPSU_SETUP			"setup"
#define	RPSU_LEVEL			"level"
#define	RPSU_DELAY			"delay"
#define	RPSU_SOURCE			"source"
#define	RPSU_DIR			"direction"
#define	RPSU_SAMPLING		"sampling"
#define	RPSU_COMMAND		"command"
#define	RPSU_START			"start"
#define	RPSU_STOP			"stop"
#define	RPSU_OUTPUT			"output"
#define	RPSU_SAMPLES		"samples"
#define	RPSU_SAMPLE_LEN		"sample_length" 
#define	RPSU_SAMPLE_TIME	"sample_time"
#define	RPSU_RET_PORT		"return_port"
#define	RPSU_RET_RESULTS	"results"
#define	RPSU_PSD_THRESHOLD	"threshold"
#define	RPSU_PSD_SHORT		"short"
#define	RPSU_PSD_LONG		"long"
#define	RPSU_PSD			"psd"
#define	RPSU_RATE			"rate"
#define	RPSU_CHANNEL		"channel"
#define	RPSU_DECIMATION		"decimation"
#define	RPSU_TRIGGER		"trigger"

//-----------------------------------------------------------------------------
#endif //__CONST_STRINGS
