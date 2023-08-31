//----------------------------------------------------------------------------
// Copyright Persistant Studios, SARL. All Rights Reserved. https://www.popcornfx.com/terms-and-conditions/
//----------------------------------------------------------------------------
#include "precompiled.h"
#include "RenderAPI_Data.h"
#include "PKUnity_Scene.h"
#include "RuntimeManager.h"

#if (PK_BUILD_WITH_GL_SUPPORT != 0)
#	include "gl_utils.h"

//-------------------------------------------------------------------------------------

using namespace PopcornFX;

extern IRenderAPIData	*CreateGLData();

class	GLData : public IRenderAPIData
{
public :
	virtual void	*BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32 fullSize, u32 mapSize);
	virtual void	EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff);
	virtual void	LoadContextFunctions();
};

//-------------------------------------------------------------------------------------

IRenderAPIData	*CreateGLData()
{
	return PK_NEW(GLData);
}

//-------------------------------------------------------------------------------------

void	*GLData::BeginModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff, u32, u32)
{
	PK_SCOPEDPROFILE();

	PK_ASSERT(CRuntimeManager::Instance().IsRenderThread());

	if (bufferHandle.m_Buffer->m_DeviceLocal == null)
		return null;

	GLenum	buffType = isIdxBuff ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	u64		bufferHandle64 = (u64)bufferHandle.m_Buffer->m_DeviceLocal;

	glBindBuffer(buffType, (GLuint)bufferHandle64); GLERR;

	// This avoids lag:
	GLint size = 0;
	glGetBufferParameteriv(buffType, GL_BUFFER_SIZE, &size); GLERR;
	glBufferData(buffType, size, null, GL_DYNAMIC_DRAW); GLERR;

	void	*mapped = glMapBuffer(buffType, GL_WRITE_ONLY); GLERR;
	return mapped;
}

void	GLData::EndModifyNativeBuffer(SBufferHandles &bufferHandle, bool isIdxBuff)
{
	PK_SCOPEDPROFILE();

	PK_ASSERT(CRuntimeManager::Instance().IsRenderThread());

	if (bufferHandle.m_Buffer->m_DeviceLocal == null)
		return;

	GLenum	buffType = isIdxBuff ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	unsigned long long buffer = (unsigned long long)bufferHandle.m_Buffer->m_DeviceLocal;

	glBindBuffer(buffType, (GLuint)(buffer)); GLERR;
	glUnmapBuffer(buffType); GLERR;
}

#if defined(PK_LINUX)

#	include <GL/glxew.h>

#elif defined(PK_MACOSX)

#	import <stdlib.h>
#	import <string.h>
#	include <AvailabilityMacros.h>	// for MAC_OS_X_VERSION_10_3

#if	defined(MAC_OS_X_VERSION_10_3)
#	import <dlfcn.h>

void	*NSGLGetProcAddress(const char *name)
{
	static void	*image = null;
	if (image == null)
	{
		image = dlopen("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", RTLD_LAZY);
		if (image == null)
			return null;
	}

	void	*addr = dlsym(image, name);
	if (addr != null)
		return addr;
#ifdef GLEW_APPLE_GLX
	return dlGetProcAddress((const GLubyte*)name); // try next for glx symbols
#else
	return null;
#endif
}

#else	// !defined(MAC_OS_X_VERSION_10_3)
#	import <mach-o/dyld.h>

void	*NSGLGetProcAddress(const char *name)
{
	static const struct mach_header	*image = null;
	if (image == null)
		image = NSAddImage("/System/Library/Frameworks/OpenGL.framework/Versions/Current/OpenGL", NSADDIMAGE_OPTION_RETURN_ON_ERROR);

	/* prepend a '_' for the Unix C symbol mangling convention */
	char	*symbolName = (char*)malloc(strlen(name) + 2);
	strcpy(symbolName+1, name);
	symbolName[0] = '_';
	NSSymbol	symbol = null;
//	if (NSIsSymbolNameDefined(symbolName))
//		symbol = NSLookupAndBindSymbol(symbolName);
	symbol = (image != null) ? NSLookupSymbolInImage(image, symbolName, NSLOOKUPSYMBOLINIMAGE_OPTION_BIND | NSLOOKUPSYMBOLINIMAGE_OPTION_RETURN_ON_ERROR) : null;
	free(symbolName);
	if (symbol != null)
		return NSAddressOfSymbol(symbol);
#ifdef GLEW_APPLE_GLX
	return dlGetProcAddress((const GLubyte*)name); // try next for glx symbols
#else
	return null;
#endif
}
#endif /* MAC_OS_X_VERSION_10_3 */

#endif

void	GLData::LoadContextFunctions()
{
#if defined(PK_LINUX)
	// Could be replaced by a glewInit() call but as it is only necessary for linux and for a few functions we do it manually:
	if (glBindBuffer == null)
	{
		glBindBuffer = (PFNGLBINDBUFFERPROC)glXGetProcAddress((const GLubyte*)"glBindBuffer");
	}
	if (glGetBufferParameteriv == null)
	{
		glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)glXGetProcAddress((const GLubyte*)"glGetBufferParameteriv");
	}
	if (glBufferData == null)
	{
		glBufferData = (PFNGLBUFFERDATAPROC)glXGetProcAddress((const GLubyte*)"glBufferData");
	}
	if (glMapBuffer == null)
	{
		glMapBuffer = (PFNGLMAPBUFFERPROC)glXGetProcAddress((const GLubyte*)"glMapBuffer");
	}
	if (glUnmapBuffer == null)
	{
	glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)glXGetProcAddress((const GLubyte*)"glUnmapBuffer");
	}
#elif defined(PK_WINDOWS)
	// Could be replaced by a glewInit() call but as it is only necessary for Windows and for a few functions we do it manually:
	if (glBindBuffer == null)
	{
		glBindBuffer = (PFNGLBINDBUFFERPROC)wglGetProcAddress("glBindBuffer");
	}
	if (glGetBufferParameteriv == null)
	{
		glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)wglGetProcAddress("glGetBufferParameteriv");
	}
	if (glBufferData == null)
	{
		glBufferData = (PFNGLBUFFERDATAPROC)wglGetProcAddress("glBufferData");
	}
	if (glMapBuffer == null)
	{
		glMapBuffer = (PFNGLMAPBUFFERPROC)wglGetProcAddress("glMapBuffer");
	}
	if (glUnmapBuffer == null)
	{
		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)wglGetProcAddress("glUnmapBuffer");
	}
#elif defined(PK_MACOSX)
	if (glBindBuffer == null)
	{
		glBindBuffer = (PFNGLBINDBUFFERPROC)NSGLGetProcAddress("glBindBuffer");
	}
	if (glGetBufferParameteriv == null)
	{
		glGetBufferParameteriv = (PFNGLGETBUFFERPARAMETERIVPROC)NSGLGetProcAddress("glGetBufferParameteriv");
	}
	if (glBufferData == null)
	{
		glBufferData = (PFNGLBUFFERDATAPROC)NSGLGetProcAddress("glBufferData");
	}
	if (glMapBuffer == null)
	{
		glMapBuffer = (PFNGLMAPBUFFERPROC)NSGLGetProcAddress("glMapBuffer");
	}
	if (glUnmapBuffer == null)
	{
		glUnmapBuffer = (PFNGLUNMAPBUFFERPROC)NSGLGetProcAddress("glUnmapBuffer");
	}
#endif
}

#endif //(PK_BUILD_WITH_GL_SUPPORT != 0)

//-------------------------------------------------------------------------------------
