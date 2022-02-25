//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RenderAPI_GLESData.h"
#include "PKUnity_Scene.h"
#include "RuntimeManager.h"

#if (PK_BUILD_WITH_GLES3_SUPPORT != 0)

#	include "gles_utils.h"

//-------------------------------------------------------------------------------------

using namespace PopcornFX;

class	GLES3Data : public IRenderAPIData
{
public:
	GLES3Data();

	virtual void	BeginFrame();
	virtual void	*BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize);
	virtual void	EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff);
};

//-------------------------------------------------------------------------------------

IRenderAPIData	*CreateGLES3Data()
{
	return PK_NEW(GLES3Data);
}

//-------------------------------------------------------------------------------------

GLES3Data::GLES3Data()
{
}

void	GLES3Data::BeginFrame()
{
	// Do NOT remove this. Seriously.
	// We seem to get errors from Unity upstream, which compromises
	// any gl error detection down the line
	glClearErrors();
}

//-------------------------------------------------------------------------------------

void	*GLES3Data::BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize)
{
	PK_SCOPEDPROFILE();
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());

	GLenum	buffType = isIdxBuff ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	glBindBuffer(buffType, (GLuint)(size_t)bufferHandle.m_Buffer->m_DeviceLocal); GLERR;

	glBufferData(buffType, fullSize, null, GL_DYNAMIC_DRAW);
	void	*mapped = glMapBufferRange(buffType, 0, mapSize, GL_MAP_WRITE_BIT | GL_MAP_INVALIDATE_BUFFER_BIT); GLERR;
	return mapped;
}

//-------------------------------------------------------------------------------------

void	GLES3Data::EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff)
{
	PK_SCOPEDPROFILE();
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());

	GLenum	buffType = isIdxBuff ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	glBindBuffer(buffType, (GLuint)(size_t)bufferHandle.m_Buffer->m_DeviceLocal); GLERR;

	glUnmapBuffer(buffType);
}

#endif //(PK_BUILD_WITH_GLES3_SUPPORT != 0)

//-------------------------------------------------------------------------------------
