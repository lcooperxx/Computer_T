// glTime.cpp: implementation of the CglTime class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "total.h"
#include "Mmsystem.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CglTime::CglTime()
{
	m_lasttime = timeGetTime();
	m_dSimuTime = 0;
	m_SimuTimeRatio = 1;
	SYSTEMTIME systime;
	GetSystemTime(&systime);
	m_SimuTimeStart = systime;
}

CglTime::~CglTime()
{

}

void CglTime::SetSimuTimeStartTime(CTime *SimuTimeStart)
{
	if (SimuTimeStart)
		m_SimuTimeStart = *SimuTimeStart;
	else
	{
		SYSTEMTIME systime;
		GetSystemTime(&systime);
		m_SimuTimeStart = systime;
	}
}
CTime& CglTime::GetSimuTimeCur()
{
	m_SimuTimeCur = m_SimuTimeStart + int(m_dSimuTime / 1000);
	return m_SimuTimeCur;
}

double CglTime::GetSimuTime()
{
	return m_dSimuTime;
}

void CglTime::SimuStart()
{
	m_lasttime = timeGetTime();
	m_dSimuTime = 0;
}

void CglTime::SimuProcess()
{
	int curtime = timeGetTime();
	m_dTimeSpan = m_SimuTimeRatio*(curtime - m_lasttime);
	m_dSimuTime += m_dTimeSpan;
	m_lasttime = curtime;
}

double CglTime::GetTimeSpan()
{
	return m_dTimeSpan;
}
