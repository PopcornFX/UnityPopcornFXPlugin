//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "PKUnity_Log.h"

__PK_API_BEGIN
//----------------------------------------------------------------------------

CUnityLog::CUnityLog()
:	m_MaxLogsCount(512)
,	m_CurrentLogWrite(&m_LogStackMain)
,	m_IgnoreLogs(false)
{
}

//----------------------------------------------------------------------------

CUnityLog::~CUnityLog()
{

}

//----------------------------------------------------------------------------

void	CUnityLog::Notify(CLog::ELogLevel level, CGuid logClass, const char *message)
{
	PK_SCOPEDLOCK(m_Lock);
	(void)logClass;
	while (m_CurrentLogWrite->Count() >= m_MaxLogsCount && m_CurrentLogWrite->Count() != 0)
		m_CurrentLogWrite->PopFrontAndDiscard();
	if (m_IgnoreLogs)
		return;
	SPopcornLogData	log = { message, level };
	m_CurrentLogWrite->PushBack(log);
}

//----------------------------------------------------------------------------

s32		CUnityLog::PopFront(SPopcornLogData &out)
{
	PK_SCOPEDLOCK(m_Lock);
	if (m_LogStackMain.Empty())
		return -1;
	out = m_LogStackMain.PopFront();
	return m_LogStackMain.Count();
}

//----------------------------------------------------------------------------

void	CUnityLog::SetMaxLogsCount(u32 maxLogsCount)
{
	PK_SCOPEDLOCK(m_Lock);
	m_LogStackMain.Clear();
	m_LogStackTmp.Clear();
	m_MaxLogsCount = maxLogsCount;
}

//----------------------------------------------------------------------------

void	CUnityLog::ClearLogs()
{
	PK_SCOPEDLOCK(m_Lock);
	m_LogStackMain.Clear();
	m_LogStackTmp.Clear();
}

//----------------------------------------------------------------------------

void	CUnityLog::StartLogs()
{
	PK_SCOPEDLOCK(m_Lock);
	m_CurrentLogWrite = &m_LogStackTmp;
}

//----------------------------------------------------------------------------

void	CUnityLog::ValidateLogsAtLevel(CLog::ELogLevel level, const CString &header)
{
	PK_SCOPEDLOCK(m_Lock);

	SPopcornLogData		mergedLogs;
	mergedLogs.m_Level = level;
	mergedLogs.m_Message += GetPrefixFromLogLevel(level) + header + "\n";
	// Get the highest log level:
	for (u32 i = 0; i < m_LogStackTmp.Count(); ++i)
	{
		mergedLogs.m_Message += GetPrefixFromLogLevel(m_LogStackTmp[i].m_Level) + m_LogStackTmp[i].m_Message + "\n";
	}
	m_LogStackTmp.Clear();
	m_CurrentLogWrite = &m_LogStackMain;
	while (m_CurrentLogWrite->Count() >= m_MaxLogsCount && m_CurrentLogWrite->Count() != 0)
		m_CurrentLogWrite->PopFrontAndDiscard();
	m_CurrentLogWrite->PushBack(mergedLogs);
}

//----------------------------------------------------------------------------

void	CUnityLog::ValidateLogs()
{
	PK_SCOPEDLOCK(m_Lock);

	TArray<SPopcornLogData>		mergedLogs;

	// Get the highest log level:
	for (u32 i = 0; i < m_LogStackTmp.Count(); ++i)
	{
		if (mergedLogs.Empty())
		{
			mergedLogs.PushBack();
			mergedLogs[mergedLogs.Count() - 1].m_Level = m_LogStackTmp[i].m_Level;
		}
		u32	idx = mergedLogs.Count() - 1;
		if (mergedLogs[idx].m_Level != m_LogStackTmp[i].m_Level)
		{
			mergedLogs.PushBack();
			idx = mergedLogs.Count() - 1;
			mergedLogs[idx].m_Level = m_LogStackTmp[i].m_Level;
		}
		mergedLogs[idx].m_Message += GetPrefixFromLogLevel(m_LogStackTmp[i].m_Level) +  m_LogStackTmp[i].m_Message + "\n";
	}
	m_LogStackTmp.Clear();
	m_CurrentLogWrite = &m_LogStackMain;
	while (m_CurrentLogWrite->Count() >= m_MaxLogsCount && m_CurrentLogWrite->Count() != 0)
		m_CurrentLogWrite->PopFrontAndDiscard();
	while (mergedLogs.Count() != 0)
		m_CurrentLogWrite->PushBack(mergedLogs.PopFront());
}

//----------------------------------------------------------------------------

void	CUnityLog::InvalidateLogs()
{
	PK_SCOPEDLOCK(m_Lock);
	m_LogStackTmp.Clear();
	m_CurrentLogWrite = &m_LogStackMain;
}

//----------------------------------------------------------------------------

void	CUnityLog::StartIgnoringLogs()
{
	PK_SCOPEDLOCK(m_Lock);
	m_IgnoreLogs = true;
}

//----------------------------------------------------------------------------

void	CUnityLog::StopIgnoringLogs()
{
	PK_SCOPEDLOCK(m_Lock);
	m_IgnoreLogs = false;
}

//----------------------------------------------------------------------------

const char	*CUnityLog::GetPrefixFromLogLevel(CLog::ELogLevel lvl)
{
	switch (lvl)
	{
	case PopcornFX::CLog::Level_Debug:
		return "[DEBUG] ";
	case PopcornFX::CLog::Level_Info:
		return "[INFO] ";
	case PopcornFX::CLog::Level_Warning:
		return "[WARN] ";
	case PopcornFX::CLog::Level_Error:
		return "[ERROR] ";
	case PopcornFX::CLog::Level_ErrorCritical:
		return "[CRITICAL ERROR] ";
	case PopcornFX::CLog::Level_ErrorInternal:
		return "[INTERNAL] ";
	default:
		return "[???] ";
	}
}

//----------------------------------------------------------------------------
__PK_API_END
