//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#pragma once

#ifndef	__SINGLE_THREADED_POOL_H__
#define	__SINGLE_THREADED_POOL_H__

#include <pk_kernel/include/kr_thread_pool.h>
#include <pk_kernel/include/kr_thread_pool_details.h>
#include <pk_kernel/include/kr_caps_cpu.h>

__PK_API_BEGIN
//----------------------------------------------------------------------------

class	CSingleThreadedPool : public Threads::CAbstractPool
{
public:
	CSingleThreadedPool()
		: m_Context(this, 1)
		, m_RecursiveCall(false)
	{
	}

	virtual ~CSingleThreadedPool()
	{
	}

	// These can be called by tasks to enqueue other dependent tasks
	virtual void	SubmitTask(Threads::CAbstractTask *task) override
	{
		if (!PK_VERIFY(task != null))
			return;
		if (!PK_VERIFY(m_Tasks.PushBack(task).Valid()))
			return;

		const CPU::CScoped_FpuDisableExceptions	_de;
		const CPU::CScoped_FpuEnableFastMode	_ftz;

		if (m_RecursiveCall == false)
		{
			m_RecursiveCall = true;
			for (u32 i = 0; i < m_Tasks.Count(); ++i)
				m_Tasks[i]->Run(m_Context);
			m_RecursiveCall = false;
			m_Tasks.Clear();
		}
	}

	virtual	CStringView	ThreadPoolName() const override { return "SingleThreaded"; };

private:
	Threads::SThreadContext				m_Context;
	TArray<Threads::PAbstractTask>		m_Tasks;

	bool								m_RecursiveCall;
};
PK_DECLARE_REFPTRCLASS(SingleThreadedPool);

//----------------------------------------------------------------------------
__PK_API_END

#endif	// __SINGLE_THREADED_POOL_H__
