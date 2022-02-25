//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"

#include <pk_kernel/include/kr_scheduler.h>

#include "PKUnity_Tasks.h"

//----------------------------------------------------------------------------

void	CWaitObject_SpinToTarget::Wait()
{
	PK_ASSERT(!m_Finished);

	u32		itCount = 0;
	while (*m_TargetCount != m_DoneCount->Load())
	{
		CCurrentThread::Yield(itCount++);
	}
	PK_ONLY_IF_ASSERTS(m_Finished = true);
	return;
}

//----------------------------------------------------------------------------

bool	CWaitObject_SpinToTarget::TryWait()
{
	PK_ASSERT(!m_Finished);

	if (*m_TargetCount != m_DoneCount->Load())
		return false;
	PK_ONLY_IF_ASSERTS(m_Finished = true);
	return true;
}

//----------------------------------------------------------------------------

bool	CWaitObject_SpinToTarget::TimedWait(u32 milliseconds)
{
	PK_ASSERT(!m_Finished);
	if (*m_TargetCount != m_DoneCount->Load())
	{
		const float	timeout = milliseconds * 1000.f;
		CTimer		timer;
		u32			itCount = 0;

		timer.Start();
		while (*m_TargetCount != m_DoneCount->Load())
		{
			if (timer.Read() >= timeout)
				return false;
			CCurrentThread::Yield(itCount++);
		}
	}
	PK_ONLY_IF_ASSERTS(m_Finished = true);
	return true;
}

void	SpinWaitDoneCount(const TAtomic<u32> &doneCount, const u32 &targetDoneCount)
{
	if (targetDoneCount != doneCount.Load())
	{
		CWaitObject_SpinToTarget		waitObject(&targetDoneCount, &doneCount, "ParticleTask::SpinWaitDoneCount");
		// WaitForObject will be able to process tasks, so it will not be "hard" spin busy wait
		Scheduler::ThreadPool()->WaitForObject(waitObject);
		PK_ASSERT(waitObject.Finished());
		PK_ASSERT(doneCount.Load() == targetDoneCount);
	}
}
