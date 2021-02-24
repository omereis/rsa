/*****************************************************************************\
|                                rp_signal.h                                  |
\*****************************************************************************/
#ifndef	__SIGNAL_H_INC
#define	__SIGNAL_H_INC

#include "rsa.h"

class TRpSignal {
public:
	TRpSignal ();
	TRpSignal (float *afSignal, int nSamples);
	TRpSignal (const TRpSignal &other);
	TRpSignal operator= (const TRpSignal & other);
	bool operator== (const TRpSignal &other) const;
	bool operator!= (const TRpSignal &other) const;
	void Clear ();
// getters/setters
	bool GetPsdDone() const;
	void SetPsdDone (bool fDone=true);
//-----------------
	bool GetMcaDone() const;
	void SetMcaDone (bool fDone=true);
//-----------------
	int GetSamples(TFloatVector &vOut) const;
	void SetSamples (const TFloatVector &vIn);
	void SetSamples (const float afSamples[], int nLength);
	int GetSamplesToBuffer(float *afSamples, int nSampleLength) const;
protected:
	void AssignAll (const TRpSignal &other);
private:
	TFloatVector m_fvSamples;
	bool m_fPsdDone;
	bool m_fMcaDone;
};

#endif //__SIGNAL_H_INC
