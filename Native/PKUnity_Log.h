#pragma once

//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include <pk_kernel/include/kr_log.h>
#include <pk_kernel/include/kr_threads.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CUnityLog : public ILogListener
{
public:
	CUnityLog();
	virtual ~CUnityLog();

	virtual void	Notify(CLog::ELogLevel level, CGuid logClass, const char *message) override;

	struct SPopcornLogData
	{
		CString					m_Message;
		CLog::ELogLevel			m_Level;
	};

	s32				PopFront(SPopcornLogData &out);
	void			SetMaxLogsCount(u32 maxLogsCount);
	void			ClearLogs();

	void			StartLogs();
	void			ValidateLogs();
	void			InvalidateLogs();

	void			StartIgnoringLogs();
	void			StopIgnoringLogs();

private:
	static const char	*GetPrefixFromLogLevel(CLog::ELogLevel lvl);

	Threads::CCriticalSection			m_Lock;
	u32									m_MaxLogsCount;
	TArray<SPopcornLogData>				m_LogStackMain;
	TArray<SPopcornLogData>				m_LogStackTmp;
	TArray<SPopcornLogData>				*m_CurrentLogWrite;
	bool								m_IgnoreLogs;
};

//----------------------------------------------------------------------------
__PK_API_END
