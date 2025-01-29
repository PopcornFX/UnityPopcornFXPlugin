//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include <pkapi/include/pk_precompiled_default.h>

#if	defined(PK_LINUX) || defined(PK_MACOSX)
#	include <stdio.h>
#	include <unistd.h>
#endif

#if defined(PK_MACOSX)
#	include <signal.h>
#endif

#include "FxStartup.h"

#include <pk_kernel/include/kr_init.h>
#include <pk_base_object/include/hb_init.h>
#include <pk_engine_utils/include/eu_init.h>
#include <pk_compiler/include/cp_init.h>
#include <pk_imaging/include/im_init.h>
#include <pk_geometrics/include/ge_init.h>
#include <pk_particles/include/ps_init.h>
#include <pk_particles_toolbox/include/pt_init.h>
#include <pk_render_helpers/include/rh_init.h>
#include <pk_kernel/include/kr_assert_internals.h>

#include <pk_kernel/include/kr_static_config_flags.h>
#include <pk_kernel/include/kr_call_context.h>
#include <pk_kernel/include/kr_log_listeners.h>
#include <pk_kernel/include/kr_log_listeners_file.h>
#include <pk_kernel/include/kr_thread_pool_default.h>
#include <pk_version_base.h>

#if defined(PK_USE_JSONSERIALIZER)
#	include <json_hbo_serializer.h>
#endif

#if defined(PK_NX)
#	define	LOG_FILE "cache:/popcorn.htm"
#else
#	define	LOG_FILE "popcorn.htm"
#endif

#ifndef PK_RETAIL
#	define	CATCH_ASSERTS		1
#else
#	define	CATCH_ASSERTS		0
#endif

PK_LOG_MODULE_DECLARE();

//----------------------------------------------------------------------------

// Use PKAssertImpl
#if	(PK_ASSERTS_IN_DEBUG != 0) || (PK_ASSERTS_IN_RELEASE != 0)
#	if	(defined(PK_WINAPI) && !defined(PK_DURANGO)) && ((PK_ASSERTS_IN_DEBUG != 0) || (PK_ASSERTS_IN_RELEASE != 0))
#		define USE_DEFAULT_ASSERT_IMPL		1
#	endif
#endif

#if !defined(PK_USE_RENDER_HELPERS)
#	define PK_USE_RENDER_HELPERS	1
#endif

//----------------------------------------------------------------------------
namespace	PKFX
{
	//----------------------------------------------------------------------------
	//
	// Assert Catcher
	//
	//----------------------------------------------------------------------------

#if (CATCH_ASSERTS == 0)

	PK_NAMESPACE::Assert::EResult	AssertCatcher_IgnoreALL(PK_ASSERT_CATCHER_PARAMETERS)
	{
		PK_ASSERT_CATCHER_KILLARGS;
		return Assert::Result_Ignore;
	}

#else // if (CATCH_ASSERTS != 0)

	//----------------------------------------------------------------------------
	//
	// This function can assert !
	// so, requires external recursion protection, ie: ASSERT_CATCHER_SCOPED_PROTECTION();
	//

	TAtomic<u32>		g_GlobalAssertId = 0;
	void	PrettyFormatAssert_Unsafe(char *buffer, u32 bufferLen, PK_ASSERT_CATCHER_PARAMETERS)
	{
		const CThreadID	threadId = CCurrentThread::ThreadID();
		const u32		globalAssertId = g_GlobalAssertId.FetchInc();
		// pointer compare ok (compiler probably removed duplicated static strings, else we dont care)
		const bool		pexp = (expanded != failed);
		const bool		pmsg = (message != failed && message != expanded);

#if	0//(PK_CALL_SCOPE_ENABLED != 0)	// pretty dangerous: CStrings can assert too, and we have no recursive protection here
		CString		callCtxStr = CCallContext::ReadStackToString();
		const char	*callCtxStrPtr = callCtxStr.Data();
#else
		const char	*callCtxStrPtr = null;
#endif	//(PK_CALL_SCOPE_ENABLED != 0)

		SNativeStringUtils::SPrintf(buffer, bufferLen,
									"!! PopcornFX Assertion failed !!\n"
									"File       : %s(%d)\n"
									"Function   : %s(...)\n"
									"%s%s\n"	// Message
									"Condition  : %s"
									"%s%s\n"	// Expanded
									"ThreadID   : %d\n"
									"AssertNum  : %d\n"
									"%s%s",		// Call context
									file, line,
									function,
									(pmsg	? "\nMessage    : " : ""), (pmsg ? message : ""),
									failed,
									(pexp	? "\nExpanded   : " : ""), (pexp ? expanded : ""),
									u32(threadId),
									globalAssertId,
									(callCtxStrPtr != null ? "\nCallContext:" : ""), (callCtxStrPtr != null ? callCtxStrPtr : ""));
	}

	//----------------------------------------------------------------------------

	static void		_LogAssertFunc_CLog(const char *str) { CLog::Log(PK_ERROR, "%s", str); }

#if	defined(PK_LINUX) || defined(PK_MACOSX)
	static void		_LogAssertFunc_CLog_And_RawError(const char *str) { _LogAssertFunc_CLog(str); fprintf(stderr, "%s\n", str); }
#endif

	static void		PrettyLogAssert_Unsafe(void (*cbLogAssertFunc)(const char *), PK_ASSERT_CATCHER_PARAMETERS)
	{
		char			_buffer[2048];
		PrettyFormatAssert_Unsafe(_buffer, sizeof(_buffer), PK_ASSERT_CATCHER_ARGUMENTS);
		cbLogAssertFunc(_buffer);
	}

	//----------------------------------------------------------------------------

	struct	SScopedAssertCatcherProtection
	{
		static PK_NAMESPACE::Threads::CCriticalSection	s_AssertProtection_Lock;
		static volatile bool							s_AssertProtection_Asserting;

		s32		m_Return;
		SScopedAssertCatcherProtection(PK_ASSERT_CATCHER_PARAMETERS)
		:	m_Return(-1)
		{
			PK_ASSERT_CATCHER_KILLARGS;
			s_AssertProtection_Lock.Lock();
			if (s_AssertProtection_Asserting)
			{
				CLog::Log(PK_ERROR, "!! ASSERT RECURSION !! %s %s(%d)", message, file, line);
				m_Return = Assert::Result_Skip;
				return;
			}
			s_AssertProtection_Asserting = true;
		}
		~SScopedAssertCatcherProtection()
		{
			s_AssertProtection_Asserting = false;
			s_AssertProtection_Lock.Unlock();
		}
	};
	PK_NAMESPACE::Threads::CCriticalSection	SScopedAssertCatcherProtection::s_AssertProtection_Lock;
	volatile bool							SScopedAssertCatcherProtection::s_AssertProtection_Asserting = false;

#define ASSERT_CATCHER_SCOPED_PROTECTION()								\
	SScopedAssertCatcherProtection		_protection(PK_ASSERT_CATCHER_ARGUMENTS); \
	if (_protection.m_Return >= 0)										\
		return static_cast<PK_NAMESPACE::Assert::EResult>(_protection.m_Return); \

	//----------------------------------------------------------------------------

	PK_NAMESPACE::Assert::EResult	AssertCatcher_LogOnly(PK_ASSERT_CATCHER_PARAMETERS)
	{
		ASSERT_CATCHER_SCOPED_PROTECTION();

		PrettyLogAssert_Unsafe(_LogAssertFunc_CLog, PK_ASSERT_CATCHER_ARGUMENTS);

		PK_NAMESPACE::Assert::EResult		result;

#ifdef PK_DEBUG
		// always print
		result = Assert::Result_Skip; // just skip this time, don't break
#else
		CLog::Log(PK_ERROR, "This assert will be ignored next time");
		result = Assert::Result_Ignore; // skip and ignore next time
#endif

		//result = Assert::Result_Break; // break point in the code
		return result;
	}

	//----------------------------------------------------------------------------

#if defined(PK_MACOSX)
	Assert::EResult		AssertCatcher_LogThenNativeOsxAlert(PK_ASSERT_CATCHER_PARAMETERS)
	{
		ASSERT_CATCHER_SCOPED_PROTECTION();
		char			_buffer[2048];
		PrettyFormatAssert_Unsafe(_buffer, sizeof(_buffer), PK_ASSERT_CATCHER_ARGUMENTS);
		_LogAssertFunc_CLog(_buffer);

		int				pipeFds[2] = { -1, -1 };
		pid_t			pId = -1;
		int				status = 0;
		Assert::EResult	result = Assert::EResult::Result_Skip;

		status = pipe(pipeFds);
		if (status == 0)
		{
			pId = fork();
			if (pId == 0) // child
			{
				char cmd[2048 + 512];

				close(pipeFds[0]);
				if (dup2(pipeFds[1], 1) == -1)
				{
					perror("PKFX Assert dup2 : ");
					close(pipeFds[1]);
					exit(1);
				}

				for (int i = 0; i < 2048; ++i)
				{
					if (_buffer[i] == '\"')
						_buffer[i] = '\'';
				}

				SNativeStringUtils::SPrintf(cmd,
											"set myReply to button returned of "
											"(display dialog \"%s\" default button 2 buttons {\"Break\", \"Skip\", \"Ignore\"})",
											_buffer);
				execl(	"/usr/bin/osascript", "osascript", "-e", "tell application \"System Events\"",
						"-e", "activate",
						"-e", cmd,
						"-e", "end tell",
						null);

				perror("PKFX Assert exec osascript");
				exit(1);
			}
			else if (pId > 0) // parent
			{
				char ret[2];

				close(pipeFds[1]);
				pipeFds[1] = -1;
				waitpid(pId, &status, WUNTRACED);
				if (status == 0)
				{
					status = read(pipeFds[0], &ret, 2);
					if (status == 2)
					{
						if (ret[0] == 'B' && ret[1] == 'r')
							result = Assert::EResult::Result_Break;
						else if (ret[0] == 'S' && ret[1] == 'k')
							result = Assert::EResult::Result_Skip;
						else if (ret[0] == 'I' && ret[1] == 'g')
							result = Assert::EResult::Result_Ignore;
						else
							perror("PKFX Assert wrong retcode");
					}
					else if (status >= 0)
					{
						perror("PKFX Assert read");
						fprintf(stderr, "read %d bytes\n", status);
					}
					else
						perror("PKFX Assert read failed");
				}
				else
				{
					kill(pId, SIGKILL);
					perror("PKFX Assert waitpid");
				}
			}
			else
				perror("PKFX Assert fork");
		}
		else
			perror("PKFX Assert pipe");

		if (pipeFds[0] != -1)
			close(pipeFds[0]);
		if (pipeFds[1] != -1)
			close(pipeFds[1]);
		return result;
	}
#endif // defined(PK_MACOSX)

	//----------------------------------------------------------------------------

#if	defined(PK_LINUX) || defined(PK_MACOSX)

	//----------------------------------------------------------------------------

	const char			*g_AssertScriptFilePath = 0;

	//----------------------------------------------------------------------------

	Assert::EResult		AssertCatcher_BreakForever(PK_ASSERT_CATCHER_PARAMETERS)
	{
		ASSERT_CATCHER_SCOPED_PROTECTION();
		PrettyLogAssert_Unsafe(_LogAssertFunc_CLog, PK_ASSERT_CATCHER_ARGUMENTS);
		return Assert::Result_Break;
	}

	//----------------------------------------------------------------------------

	Assert::EResult		AssertCatcher_ExecScript(PK_ASSERT_CATCHER_PARAMETERS)
	{
		ASSERT_CATCHER_SCOPED_PROTECTION();

		PK_NAMESPACE::Assert::EResult		result = Assert::Result_Skip;

		PrettyLogAssert_Unsafe(_LogAssertFunc_CLog, PK_ASSERT_CATCHER_ARGUMENTS);

		char		cmd[2048];
		SNativeStringUtils::SPrintf(cmd, "%s %d \"%s\" \"%s\" \"%d\" \"%s\" \"%s\" \"%s\"",
									g_AssertScriptFilePath, getpid(), PK_ASSERT_CATCHER_ARGUMENTS);
		int			res = system(cmd);
		if (res >= 0)
		{
			if (WIFSIGNALED(res))
			{
				Assert::SetCatcher(&AssertCatcher_BreakForever);
				result = Assert::Result_Break;
			}
			else
			{
				int		status = WEXITSTATUS(res);
				switch (status)
				{
				case 1: result = Assert::Result_Break; break;
				case 2: result = Assert::Result_Ignore; break;
				case 3:
					Assert::SetCatcher(&AssertCatcher_BreakForever);
					result = Assert::Result_Break;
					break;
				}
			}
		}
		return result;
	}

	//----------------------------------------------------------------------------

	Assert::EResult	AssertCatcher_AskTTY(PK_ASSERT_CATCHER_PARAMETERS)
	{
		ASSERT_CATCHER_SCOPED_PROTECTION();

		PK_NAMESPACE::Assert::EResult		result = Assert::Result_Skip;

		// also printf beacuse CLog could be a log file only
		PrettyLogAssert_Unsafe(_LogAssertFunc_CLog_And_RawError, PK_ASSERT_CATCHER_ARGUMENTS);

		fprintf(stderr, "\nWhat now ? (0:break, 1:skip, 2:ignore, 3:break forever) ");
		u32		d = 0;
		if (fscanf(stdin, "%d", &d) == EOF)
			d = 0;	// avoid a warning on gcc

		switch (d)
		{
		case 1: result = Assert::Result_Skip; break;
		case 2: result = Assert::Result_Ignore; break;
		case 3:
			Assert::SetCatcher(&AssertCatcher_BreakForever);
			result = Assert::Result_Break;
			break;
		default: result = Assert::Result_Break; break;
		}
		return result;
	}
#endif // defined(PK_LINUX) || defined(PK_MACOSX)

	//----------------------------------------------------------------------------

#if defined(USE_DEFAULT_ASSERT_IMPL)
	Assert::EResult	AssertCatcher_LogThenDefault(PK_ASSERT_CATCHER_PARAMETERS)
	{
		ASSERT_CATCHER_SCOPED_PROTECTION();
		PrettyLogAssert_Unsafe(_LogAssertFunc_CLog, PK_ASSERT_CATCHER_ARGUMENTS);
		return PKAssertImpl(PK_ASSERT_CATCHER_ARGUMENTS);
	}
#endif

#endif // (CATCH_ASSERTS != 0)

	//----------------------------------------------------------------------------
	//
	// Loggers
	//
	//----------------------------------------------------------------------------

#if	defined(PK_WINAPI)
	class	CLogListenerStdout : public ILogListener
	{
	private:
	public:
		virtual void	Notify(CLog::ELogLevel level, CGuid logClass, const char *message) override
		{
			CString	s = CString::Format("[%d][%s]> %s\n", level, CLog::LogClassToString(logClass), message);
			printf("%s", s.Data());
		}
	};
#endif

	//----------------------------------------------------------------------------

	void	AddDefaultLogListenersOverride_DefaultLogger(void *)
	{
#if	(PK_LOG_ENABLED != 0)
		CLog::AddGlobalListener(PK_NEW(CLogListenerFile(LOG_FILE, "popcorn-engine logfile")));
#	if	(PK_LOG_DEV_LOGGERS_ENABLED != 0) && !defined(PK_RETAIL)	// don't log anything to debut output in retail
		CLog::AddGlobalListener(PK_NEW(CLogListenerDebug()));
#		if	defined(PK_WINAPI)
		CLog::AddGlobalListener(PK_NEW(CLogListenerStdout));
#		endif
#	endif
#endif	// (PK_LOG_ENABLED != 0)
	}

	//----------------------------------------------------------------------------

	void	AddDefaultLogListenersOverride_NoDefaultLogger(void *)
	{
#if	(PK_LOG_ENABLED != 0)
		// only log to the LOG_FILE, not to the debug output
		CLog::AddGlobalListener(PK_NEW(CLogListenerFile(LOG_FILE, "popcorn-engine logfile")));
#endif
	}

	//----------------------------------------------------------------------------
	//
	// Worker Thread Pool
	//
	//----------------------------------------------------------------------------

	static Threads::PAbstractPool		_CreateThreadPool_Default()
	{
		PWorkerThreadPool				pool = PK_NEW(CWorkerThreadPool);
		if (!PK_VERIFY(pool != null))
			return null;
		// Uses cpu core caps to spawn 1 worker per logical core with affinity
		pool->AddNUMAWorkers();
		pool->StartWorkers();
		return pool;
	}

	//----------------------------------------------------------------------------
	//
	// Startup
	//
	//----------------------------------------------------------------------------

	bool	RuntimeStartup(const char *assertScriptFilePath /* = null*/, bool installDefaultLogger /*= true*/)
	{
		(void)assertScriptFilePath;
		SDllVersion	engineVersion;

		PK_ASSERT(engineVersion.Major == PK_VERSION_MAJOR);
		PK_ASSERT(engineVersion.Minor == PK_VERSION_MINOR);
#ifdef	PK_DEBUG
		PK_ASSERT(engineVersion.Debug == true);
#else
		PK_ASSERT(engineVersion.Debug == false);
#endif
		CPKKernel::Config	configKernel;

#if (CATCH_ASSERTS == 0)
		configKernel.m_AssertCatcher = &AssertCatcher_IgnoreALL;
#else
#	if	defined(PK_LINUX) || defined(PK_MACOSX)
		configKernel.m_AssertCatcher = null;
		g_AssertScriptFilePath = assertScriptFilePath;
		if (g_AssertScriptFilePath != 0)
		{
			if (access(g_AssertScriptFilePath, X_OK) == 0)
			{
				int		res = system(g_AssertScriptFilePath); // exec once
				if (res == 0)
				{
					printf("PKFX: Assert script catcher \"%s\" enabled\n", g_AssertScriptFilePath);
					configKernel.m_AssertCatcher = &AssertCatcher_ExecScript;
				}
				else
					fprintf(stderr, "PKFX: Assert script catcher \"%s\" not valid\n", g_AssertScriptFilePath);
			}
			else
				fprintf(stderr, "PKFX: Assert script catcher \"%s\" not found or permission denied\n", g_AssertScriptFilePath);
		}
		if (configKernel.m_AssertCatcher == null)
		{
			if (isatty(fileno(stdin)) && isatty(fileno(stdout)))
			{
				printf("PKFX: Assert Catcher will ask on the TTY !\n");
				configKernel.m_AssertCatcher = &AssertCatcher_AskTTY;
			}
			else
				configKernel.m_AssertCatcher = &AssertCatcher_LogOnly;
		}

#	elif defined(USE_DEFAULT_ASSERT_IMPL)
		// leave it as it ?
		// or use pkapi/pkapi_assert_notify_win32.cpp ?
		configKernel.m_AssertCatcher = &AssertCatcher_LogThenDefault;
#	else
		configKernel.m_AssertCatcher = &AssertCatcher_LogOnly;
#	endif
#endif // (CATCH_ASSERTS != 0)

#if	defined(PK_ORBIS)
		// Our main-thread is scheduled on core 3 (see the CCurrentThread::SetTargetProcessor() call below)
		const u32	kOrbisWorkerAffinities[] =
		{
			0x07,
			0x07,
			0x07,
//			0x0F,	// our main thread
			0x30,
			0x30,
//			0xF0,	// reserved by the system
//			0xF0,	// reserved by the system
		};
		configKernel.m_WorkerAffinityMasks = kOrbisWorkerAffinities;
		configKernel.m_WorkerAffinityMasksCount = PK_ARRAY_COUNT(kOrbisWorkerAffinities);
#endif

		const bool		forceDefaultThreadPool = false;
		if (forceDefaultThreadPool)
		{
			configKernel.m_CreateThreadPool = &_CreateThreadPool_Default;
		}

		// depending on whether or not we want default loggers, override the callback:
		if (installDefaultLogger)
			configKernel.m_AddDefaultLogListeners = &AddDefaultLogListenersOverride_DefaultLogger;
		else
			configKernel.m_AddDefaultLogListeners = &AddDefaultLogListenersOverride_NoDefaultLogger;

		HBO::ISerializer *serializers[] =
		{
#if defined(PK_USE_JSONSERIALIZER)
			new(JsonHBO::CSerializerJSON),
#endif
			new(HBO::CSerializerPKBO),
		};
		CPKBaseObject::Config  configBaseObject;
		configBaseObject.m_CustomSerializers = serializers;

		if (CPKKernel::Startup(engineVersion, configKernel) &&
			CPKBaseObject::Startup(engineVersion, configBaseObject) &&
			CPKEngineUtils::Startup(engineVersion, CPKEngineUtils::Config()) &&
			CPKCompiler::Startup(engineVersion, CPKCompiler::Config()) &&
			CPKImaging::Startup(engineVersion, CPKImaging::Config()) &&
			CPKGeometrics::Startup(engineVersion, CPKGeometrics::Config()) &&
			CPKParticles::Startup(engineVersion, CPKParticles::Config()) &&
			ParticleToolbox::Startup() &&
#if (PK_USE_RENDER_HELPERS != 0)
			CPKRenderHelpers::Startup(engineVersion, CPKRenderHelpers::Config()) &&
#endif // (PK_USE_RENDER_HELPERS != 0)
			PK_VERIFY(Kernel::CheckStaticConfigFlags(Kernel::g_BaseStaticConfig, SKernelConfigFlags())))
		{
			PK_LOG_MODULE_INIT_START;

			// Lock our main-thread on a specific core (you'll probably want to remove this in your integration)
#if	defined(PK_ORBIS)
			// Lock our main-thread on core 3 for minimal competition with system threads (you don't have to do this, we do it for this sample)
			// If your main thread is locked on another specific core, check out the 'PKFX::PopcornFXStartup()' function in 'FxStartup.cpp'
			// and make sure you change the default worker affinities so that they don't overlap with your main thread.
			CCurrentThread::SetTargetProcessor(3);
			// Boost our main thread's priority
			CCurrentThread::SetPriority(CThreadManager::Priority_High);

#elif defined(PK_CONSOLE) // dont touch affinity on desktop or mobile
			CCurrentThread::SetTargetProcessor(0);
			// Boost our main thread's priority
			CCurrentThread::SetPriority(CThreadManager::Priority_High);
#endif

			if (forceDefaultThreadPool)
			{
				CCurrentThread::SetTargetProcessor(0);
				// Boost our main thread's priority
				CCurrentThread::SetPriority(CThreadManager::Priority_High);
			}

			CThreadManager::SetProcessPriority(CThreadManager::Process_High);

			PK_LOG_MODULE_INIT_END;

			CLog::Log(PK_INFO, "Started");
			return true;
		}

		RuntimeShutdown();	// shutdown the modules we were able to load...
		return false;
	}

	//----------------------------------------------------------------------------

	void	RuntimeShutdown()
	{
		PK_LOG_MODULE_RELEASE_START;
		PK_LOG_MODULE_RELEASE_END;

#if (PK_USE_RENDER_HELPERS != 0)
		CPKRenderHelpers::Shutdown();
#endif // (PK_USE_RENDER_HELPERS != 0)

		ParticleToolbox::Shutdown();
		CPKParticles::Shutdown();
		CPKGeometrics::Shutdown();
		CPKImaging::Shutdown();
		CPKCompiler::Shutdown();
		CPKEngineUtils::Shutdown();
		CPKBaseObject::Shutdown();
		CPKKernel::Shutdown();
	}

}
//----------------------------------------------------------------------------
