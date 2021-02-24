/*****************************************************************************\
|                               rp_signal.cpp                                 |
\*****************************************************************************/
#include <string.h>
#include "rp_signal.h"
#include <algorithm>
//-----------------------------------------------------------------------------
TRpSignal::TRpSignal ()
{
	Clear ();
}
//-----------------------------------------------------------------------------
TRpSignal::TRpSignal (const TRpSignal &other)
{
	AssignAll (other);
}
//-----------------------------------------------------------------------------
TRpSignal::TRpSignal (float *afSignal, int nSamples)
{
	if ((afSignal != NULL) && (nSamples > 0))
		m_fvSamples = TFloatVector(afSignal, afSignal + nSamples);
	else
		m_fvSamples.clear();
	SetPsdDone (false);
	SetMcaDone (false);
}
//-----------------------------------------------------------------------------
TRpSignal TRpSignal::operator= (const TRpSignal & other)
{
	AssignAll (other);
	return (*this);
}
//-----------------------------------------------------------------------------
bool TRpSignal::operator== (const TRpSignal &other) const
{
	bool f;

	if (!std::equal(m_fvSamples.begin(), m_fvSamples.end(), other.m_fvSamples.begin()))
		f = false;
	else if (GetPsdDone() != other.GetPsdDone())
		f = false;
	else if (GetMcaDone() != other.GetMcaDone())
		f = false;
	else
		f = true;
	return (f);
}
//-----------------------------------------------------------------------------
bool TRpSignal::operator!= (const TRpSignal &other) const
{
	return (!(*this == other));
}
//-----------------------------------------------------------------------------
void TRpSignal::Clear ()
{
	m_fvSamples.clear();
	SetPsdDone (false);
	SetMcaDone (false);
}
//-----------------------------------------------------------------------------
bool TRpSignal::GetPsdDone() const
{
	return (m_fPsdDone);
}
//-----------------------------------------------------------------------------
void TRpSignal::SetPsdDone (bool fDone)
{
	m_fPsdDone = fDone;
}
//-----------------------------------------------------------------------------
bool TRpSignal::GetMcaDone() const
{
	return (m_fMcaDone);
}
//-----------------------------------------------------------------------------
void TRpSignal::SetMcaDone (bool fDone)
{
	m_fMcaDone = fDone;
}
//-----------------------------------------------------------------------------
int TRpSignal::GetSamples(TFloatVector &vOut) const
{
	vOut = TFloatVector(m_fvSamples.begin(), m_fvSamples.end());
	return ((int) vOut.size());
}
//-----------------------------------------------------------------------------
void TRpSignal::SetSamples (const TFloatVector &vIn)
{
	m_fvSamples = TFloatVector (vIn.begin(), vIn.end());
}
//-----------------------------------------------------------------------------
void TRpSignal::SetSamples (const float afSamples[], int nLength)
{
	m_fvSamples = TFloatVector(afSamples, afSamples + nLength);
}
//-----------------------------------------------------------------------------

void TRpSignal::AssignAll (const TRpSignal &other)
{
	m_fvSamples = TFloatVector (other.m_fvSamples.begin(), other.m_fvSamples.end());
	SetPsdDone (other.GetPsdDone());
	SetMcaDone (other.GetMcaDone());

}
//-----------------------------------------------------------------------------

int TRpSignal::GetSamplesToBuffer(float *afSamples, int nSampleLength) const
{
	int n, nCount;
	TFloatVector::const_iterator i;

	memset (afSamples, 0, nSampleLength * sizeof(afSamples[0]));
	for (i=m_fvSamples.begin(), n=0 ; (i != m_fvSamples.end()) && (n < nSampleLength) ; n++, i++)
		afSamples[n] = *i;
	return (n);
}
//-----------------------------------------------------------------------------
