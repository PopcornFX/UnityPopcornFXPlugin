#include "precompiled.h"
#include "ILoopbackCapture.h"

#if defined(PK_WINDOWS)
#	include "Windows/LoopbackCapture.h"
#endif

ILoopbackCapture *ILoopbackCapture::CreateLoopbackCapture()
{
#if defined(PK_WINDOWS)
	return PK_NEW(CWindowLoopbackCapture);
#else
	return null;
#endif
}
