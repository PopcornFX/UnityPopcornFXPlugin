//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

#include <pk_kernel/include/kr_refcounted_buffer.h>
#include <pk_kernel/include/kr_threads_job_pool.h>

#include "ImplemGraphicsAPI/RenderAPI_Data.h"

using namespace		PopcornFX;

//----------------------------------------------------------------------------

class	CWaitObject_SpinToTarget : public Threads::CAbstractWaitObject
{
public:
	CWaitObject_SpinToTarget(const u32 *targetCount, const TAtomic<u32> *doneCount, const char *prettyName)
	:	m_TargetCount(targetCount)
	,	m_DoneCount(doneCount)
	PK_ONLY_IF_ASSERTS(PK_COMMA m_Finished(false))
#	if	(KR_THREADPOOL_PRETTYDEBUG != 0)
	,	m_PrettyName(CStringView::FromNullTerminatedString(prettyName))
#	endif
	{
		PK_ASSERT(m_TargetCount != null && m_DoneCount != null && m_DoneCount->Load() != 0);
	}

	virtual void			Wait() override;
	virtual bool			TryWait() override;
	virtual bool			TimedWait(u32 milliseconds) override;

#	if	(KR_THREADPOOL_PRETTYDEBUG != 0)
	virtual CStringView		_PrettyDebugName() const override { return CStringView(m_PrettyName); }
#	endif

	PK_ONLY_IF_ASSERTS(bool	Finished() const { return m_Finished; })

protected:
	const u32			*m_TargetCount;
	const TAtomic<u32>	*m_DoneCount;
	PK_ONLY_IF_ASSERTS(bool	m_Finished;)
#	if	(KR_THREADPOOL_PRETTYDEBUG != 0)
	CStringView			m_PrettyName;
#	endif
};

void	SpinWaitDoneCount(const TAtomic<u32> &doneCount, const u32 &targetDoneCount);

//----------------------------------------------------------------------------
