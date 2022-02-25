//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------

#include "precompiled.h"
#include "RenderAPI_GLESData.h"
#include "PKUnity_Scene.h"
#include "RuntimeManager.h"

#if (PK_BUILD_WITH_GLES_SUPPORT != 0)

#	define	PK_USE_GLES2_INCLUDES
#	include "gles_utils.h"
#	undef	PK_USE_GLES2_INCLUDES

//-------------------------------------------------------------------------------------

using namespace PopcornFX;

class	GLES2Data : public IRenderAPIData
{
public:
	GLES2Data();

	virtual void	BeginFrame();
	virtual void	*BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize);
	virtual void	EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff);

private:
#if		defined(PK_ANDROID)
	PFNGLMAPBUFFEROESPROC	m_glMapBufferOES;
	PFNGLUNMAPBUFFEROESPROC	m_glUnmapBufferOES;
#endif

	bool					m_CanMapBuffer;
};

//-------------------------------------------------------------------------------------

IRenderAPIData	*CreateGLES2Data()
{
	return PK_NEW(GLES2Data);
}

//-------------------------------------------------------------------------------------

GLES2Data::GLES2Data()
{
	const char		*glExt = (char*)glGetString(GL_EXTENSIONS);
	const CString	glExtStr(glExt);

#if		defined(PK_ANDROID)
	if (glExtStr.Contains("GL_OES_mapbuffer"))
	{
		CLog::Log(PK_INFO, "Has the GL_OES_mapbuffer extension");
		m_glMapBufferOES = (PFNGLMAPBUFFEROESPROC)eglGetProcAddress("glMapBufferOES");
		m_glUnmapBufferOES = (PFNGLUNMAPBUFFEROESPROC)eglGetProcAddress("glUnmapBufferOES");
		CLog::Log(PK_INFO, "eglGetProcAddress glMapBufferOES:	%s", m_glMapBufferOES != null ? "SUCCESS" : "FAILURE");
		CLog::Log(PK_INFO, "eglGetProcAddress glUnmapBufferOES:	%s", m_glUnmapBufferOES != null ? "SUCCESS" : "FAILURE");
	}
	else
	{
		CLog::Log(PK_INFO, "Does not have the GL_OES_mapbuffer extension, will use RAM staging buffers");
		m_glMapBufferOES = null;
		m_glUnmapBufferOES = null;
	}
	m_CanMapBuffer = m_glMapBufferOES != null && m_glUnmapBufferOES != null;
#else
	m_CanMapBuffer = false;
#endif
}

//-------------------------------------------------------------------------------------

void	GLES2Data::BeginFrame()
{
	// Do NOT remove this. Seriously.
	// We seem to get errors from Unity upstream, which compromises
	// any gl error detection down the line
	glClearErrors();
}

//-------------------------------------------------------------------------------------

void	*GLES2Data::BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize)
{
	PK_SCOPEDPROFILE();
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());

	GLenum	buffType = isIdxBuff ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	glBindBuffer(buffType, (GLuint)(size_t)bufferHandle.m_Buffer->m_DeviceLocal); GLERR;

	void				*mapped = null;
	CGLESBufferHandles	*bufferHandleGLES = (CGLESBufferHandles*)(bufferHandle.m_Buffer.Get());

#if		defined(PK_ANDROID)
	if (m_CanMapBuffer)
	{
		glBufferData(buffType, fullSize, null, GL_DYNAMIC_DRAW); GLERR;
		mapped = m_glMapBufferOES(buffType, GL_WRITE_ONLY_OES); GLERR;
	}
	else
#endif
	{
		if (bufferHandleGLES->m_GLESHostVisibleSize < s32(fullSize))
		{
			if (bufferHandleGLES->m_GLESHostVisibleBuffer != null)
			{
				bufferHandleGLES->m_GLESHostVisibleBuffer = PK_REALLOC_ALIGNED(bufferHandleGLES->m_GLESHostVisibleBuffer, fullSize, 0x10);
			}
			else
			{
				bufferHandleGLES->m_GLESHostVisibleBuffer = PK_MALLOC_ALIGNED(fullSize, 0x10);
			}
			bufferHandleGLES->m_GLESHostVisibleSize = fullSize;
		}
		bufferHandleGLES->m_GLESLastMappedSize = mapSize;
		mapped = bufferHandleGLES->m_GLESHostVisibleBuffer;
	}
	return mapped;
}

//-------------------------------------------------------------------------------------

void	GLES2Data::EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff)
{
	PK_SCOPEDPROFILE();
	PK_ASSERT(CRuntimeManager::Instance().GetScene().IsRenderThread());

	GLenum	buffType = isIdxBuff ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	glBindBuffer(buffType, (GLuint)(size_t)bufferHandle.m_Buffer->m_DeviceLocal); GLERR;

	CGLESBufferHandles	*bufferHandleGLES = (CGLESBufferHandles*)(bufferHandle.m_Buffer.Get());

#if		defined(PK_ANDROID)
	if (m_CanMapBuffer)
	{
		m_glUnmapBufferOES(buffType); GLERR;
	}
	else
#endif
	{
		// Here we consider the bufferHandle.m_GLESHostVisibleSize to be the full size of the device local buffer:
		glBufferData(buffType, bufferHandleGLES->m_GLESHostVisibleSize, null, GL_DYNAMIC_DRAW);
		glBufferSubData(buffType, 0, bufferHandleGLES->m_GLESLastMappedSize, bufferHandleGLES->m_GLESHostVisibleBuffer); GLERR;
	}
}

#endif //(PK_BUILD_WITH_GLES_SUPPORT != 0)

//-------------------------------------------------------------------------------------
