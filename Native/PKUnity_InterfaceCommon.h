//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#pragma once

// MANAGED_TO_POPCORN_CONVENTION
#ifdef MANAGED_TO_POPCORN_CONVENTION
#	undef	MANAGED_TO_POPCORN_CONVENTION
#endif

#if defined(PK_ORBIS) || defined(PK_UNKNOWN2)
#	define	MANAGED_TO_POPCORN_CONVENTION			__declspec(dllexport)
#else
#	define	MANAGED_TO_POPCORN_CONVENTION			PK_API_EXPORT
#endif

// POPCORN_CALLBACK_CONVENTION
#ifdef POPCORN_TO_MANAGED_CONVENTION
#	undef	POPCORN_TO_MANAGED_CONVENTION
#endif

#if defined(PK_MACOSX) || defined(PK_LINUX) || defined(PK_ANDROID) || defined(PK_ORBIS) || defined(PK_UNKNOWN2) || defined(PK_IOS) || defined(PK_NX)
#	define POPCORN_TO_MANAGED_CONVENTION
#else
#	define POPCORN_TO_MANAGED_CONVENTION		PK_STDCALL
#endif

// We replace the booleans by this to make them 32 bits:
enum	ManagedBool : int
{
	ManagedBool_False = 0,
	ManagedBool_True = 1,
};

// Samplers type:
enum EUnitySupportedSamplerType : int
{
	SamplerShape = 0,
	SamplerCurve,
	SamplerImage,
	SamplerText,
	SamplerUnsupported
};

struct	SLogPluginCalls
{
	const char	*m_FunctionName;

	SLogPluginCalls(const char *functionName)
		: m_FunctionName(functionName)
	{
		CLog::Log(PK_INFO, "Enter call of \"%s\"", m_FunctionName);
	}

	~SLogPluginCalls()
	{
		CLog::Log(PK_INFO, "Leave call of \"%s\"", m_FunctionName);
	}
};

#define	MANAGED_BOOL_2_STR(x)	(x == ManagedBool_True ? "true" : "false")

#if		defined(__FUNCTION__) && 0 //defined(PK_DEBUG)
#	define		LOG_PLUGIN_CALL						SLogPluginCalls		__logCall(__FUNCTION__);
#else
#	define		LOG_PLUGIN_CALL
#endif // __FUNCTION__ && PK_DEBUG

#define	NEED_PK_RUNTIME_STARTED(_exp)				LOG_PLUGIN_CALL if (!CRuntimeManager::IsInstanceInitialized()) { CLog::Log(PK_WARN, "PK-Runtime not started before the call to %s", __FUNCTION__); _exp; }
#define	NEED_PK_RUNTIME_AND_EDITOR_STARTED(_exp)	NEED_PK_RUNTIME_STARTED(_exp) if (!CEditorManager::IsInstanceInitialized()) { CLog::Log(PK_WARN, "The editor manager was not started before the call to %s", __FUNCTION__); _exp; }
#define	NEED_PK_MEDIUM_COLLECTION_CREATED(_exp)		LOG_PLUGIN_CALL if (!CRuntimeManager::IsInstanceInitialized() || !CRuntimeManager::Instance().GetScene().IsMediumCollectionInitialized()) { CLog::Log(PK_WARN, "Simulation not started before the call to %s", __FUNCTION__); _exp; }
