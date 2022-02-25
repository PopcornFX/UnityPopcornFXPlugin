//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RenderAPI_MetalData.h"
#include "PKUnity_Scene.h"
#include "RuntimeManager.h"

#if (PK_BUILD_WITH_METAL_SUPPORT != 0)

#	ifndef __OBJC__
#		error metal plugin is objc code.
#	endif
#	ifndef __clang__
#		error only clang compiler is supported.
#	endif
#	if !__has_feature(objc_arc)
#		error metal demands ARC enabled.
#	endif

#	import <Metal/Metal.h>
#include "UnityGraphicsAPI/IUnityGraphicsMetal.h"
using namespace PopcornFX;

extern IRenderAPIData*	CreateMetalData();

//-------------------------------------------------------------------------------------

class MetalData : public IRenderAPIData
{
public :
	MetalData()
	:	m_CurrentBuffIdx(0)
	{
	}

	virtual void	BeginFrame();
	virtual void	EndFrame();
	virtual void	*BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize);
	virtual void	EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff);

private:
	u32				m_CurrentBuffIdx;

	id<MTLBlitCommandEncoder>	m_BlitEncoder;
};

struct 	SMetalBuff
{
	id<MTLBuffer> 	m_Buffer;
};

//-------------------------------------------------------------------------------------

IRenderAPIData	*CreateMetalData()
{
	return PK_NEW(MetalData);
}

//-------------------------------------------------------------------------------------

void	MetalData::BeginFrame()
{
	m_CurrentBuffIdx = (m_CurrentBuffIdx + 1) % SBufferHandles::HOST_VISIBLE_BUFFER_NUMBER;
}

//-------------------------------------------------------------------------------------

void	MetalData::EndFrame()
{
	[m_BlitEncoder endEncoding];
}

//-------------------------------------------------------------------------------------

void	*MetalData::BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize)
{
	(void)isIdxBuff;
	(void)fullSize;
	(void)mapSize;
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());
	CMetalBufferHandles	*bufferHandleMetal = (CMetalBufferHandles*)(bufferHandle.m_Buffer.Get());
	bufferHandleMetal->m_MappedSize = mapSize;
	id<MTLBuffer> buf = (__bridge id<MTLBuffer>)bufferHandleMetal->m_DeviceLocal;
	return [buf contents];
}

void	MetalData::EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff)
{
	(void)bufferHandle;
	(void)isIdxBuff;
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());
#if 	defined(PK_MACOSX)
	CMetalBufferHandles	*bufferHandleMetal = (CMetalBufferHandles*)(bufferHandle.m_Buffer.Get());
	id<MTLBuffer> buf = (__bridge id<MTLBuffer>)bufferHandleMetal->m_DeviceLocal;
	[buf didModifyRange:NSMakeRange(0, bufferHandleMetal->m_MappedSize)];
#endif
}

//-------------------------------------------------------------------------------------

#endif //BUILD_WITH_METAL_SUPPORT
