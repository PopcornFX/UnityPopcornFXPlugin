#pragma once

//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#ifndef __SAMPLEGL_GL_UTILS_H__
#define __SAMPLEGL_GL_UTILS_H__

#include "popcornfx.h"

#if defined(PK_MACOSX)
#	include <OpenGL/gl.h>
#	include <OpenGl/glu.h>
#	include <GLUT/glut.h>
#elif defined(PK_ANDROID)
#	if defined(PK_USE_GLES2_INCLUDES)
#		include <EGL/egl.h>			// We try to load the map / unmap extension on opengl es so we need this for GetProcAddress
#		include <GLES2/gl2.h>
#		include <GLES2/gl2ext.h>
#	else
#		include <GLES3/gl3.h>
#		include <GLES3/gl3ext.h>
#	endif
#elif defined(PK_IOS)
#	if defined(PK_USE_GLES2_INCLUDES)
#		include <OpenGLES/ES2/gl.h>
#		include <OpenGLES/ES2/glext.h>
#	else
#		include <OpenGLES/ES3/gl.h>
#		include <OpenGLES/ES3/glext.h>
#	endif
#else
#	include <GL/gl.h>
#endif

#include "printf_utils.h"

#define	_GLGETERRSTR(__enum)	{__enum, #__enum}

//----------------------------------------------------------------------------

PK_INLINE char const	*glGetFramebufferStatusStrError(size_t glError)
{
	static const struct
	{
		size_t		e;
		char const	*str;
	} errs[] =
	{
		_GLGETERRSTR(GL_FRAMEBUFFER_COMPLETE),
		_GLGETERRSTR(GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT),
		_GLGETERRSTR(GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT),
		_GLGETERRSTR(GL_FRAMEBUFFER_UNSUPPORTED),
	};

	if (glError == 0)
		return "";
	for (size_t i = 0; i < sizeof(errs) / sizeof(*errs); ++i)
	{
		if (errs[i].e == glError)
			return errs[i].str;
	}
	return "GL_FRAMEBUFFER UNKNOWN ??";
}

//----------------------------------------------------------------------------

PK_INLINE char const	*glGetStrError(size_t glError)
{
	static const struct
	{
		size_t		e;
		char const	*str;
	} errs[] =
	{
		_GLGETERRSTR(GL_NO_ERROR),
		_GLGETERRSTR(GL_INVALID_ENUM),
		_GLGETERRSTR(GL_INVALID_VALUE),
		_GLGETERRSTR(GL_INVALID_OPERATION),
		_GLGETERRSTR(GL_INVALID_FRAMEBUFFER_OPERATION),
		_GLGETERRSTR(GL_OUT_OF_MEMORY),
	};

	if (glError == 0)
		return "";
	for (size_t i = 0; i < sizeof(errs) / sizeof(*errs); ++i)
	{
		if (errs[i].e == glError)
			return errs[i].str;
	}
	return "GL_ERROR UNKNOWN ??";
}

#undef	_GLGETERRSTR
//----------------------------------------------------------------------------

PK_INLINE char const	*glGetStrError()
{
	return glGetStrError(glGetError());
}

//----------------------------------------------------------------------------
// check only on debug mode

#ifdef	_DEBUG
#	define	GLERR													\
	do {															\
		int		e = glGetError();									\
		if (e != GL_NO_ERROR)										\
			fprintf(stderr, "Error OpenGl: %s:%d %s\n",				\
					__FILE__, __LINE__, glGetStrError(e));	\
	} while (0)

#else
#	define	GLERR
#endif

//----------------------------------------------------------------------------
// always check errors:

#define	GL_ERROR_MSG(__fmt, ...)								\
	do {														\
		int		e = glGetError();								\
		if (e != GL_NO_ERROR)									\
			fprintf(stderr, "Error OpenGl: '%s' " __fmt "\n",	\
					glGetStrError(e), ##__VA_ARGS__);			\
	} while (0)

#define	GL_ERROR(__call)								\
	do {												\
		__call;											\
		int		e = glGetError();						\
		if (e != GL_NO_ERROR)							\
			fprintf(stderr, "Error OpenGl: %s (%s)\n",	\
					glGetStrError(e), #__call);			\
	} while (0)

#define	GL_IF_ERROR(__call, __do_if_error)				\
	do {												\
		__call;											\
		int		e = glGetError();						\
		if (e != GL_NO_ERROR)							\
			fprintf(stderr, "Error OpenGl: %s (%s)\n",	\
					glGetStrError(e), #__call);			\
	} while (0)

#define	GLERROR(__msg, __todo_if_error)								\
	do {															\
		int		e = glGetError();									\
		if (e != GL_NO_ERROR)										\
		{															\
		 fprintf(stderr, "Error OpenGl: %s:%d %s: %s\n",			\
					 __FILE__, __LINE__, __msg, glGetStrError(e));	\
		 __todo_if_error;											\
		 }															\
	} while (0)


//----------------------------------------------------------------------------
// Delete an opengl resource safly

#define	glDelete(__name, __glid)		do { if (__glid != 0) { glDelete ## __name (__glid); __glid = 0; } } while (0)
#define	glDeleteN(__nb, __name, __glid)	do { if (__glid != 0) { glDelete ## __name (__nb, &(__glid)); __glid = 0; } } while (0)
#define	glDelete1(__name, __glid)		do { if (__glid != 0) { glDelete ## __name (1   , &(__glid)); __glid = 0; } } while (0)

//----------------------------------------------------------------------------

PK_INLINE void		glClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

//----------------------------------------------------------------------------

bool	glPrintShaderCompileInfo(GLuint shader);
bool	glPrintProgramLinkInfo(GLuint program);
bool	glCreateCompileShader(GLenum shaderType, char const ** codes, size_t codesCount, GLuint &outShader);
bool	glCreateCompileShader(GLenum shaderType, char const * code, GLuint &outShader);
bool	glHasExtension(const char *nameStrings);
void	glEnableSrgbIFP();
void	glDisableSrgbIFP();
GLint	glSrgbTexFormatIFP();

//----------------------------------------------------------------------------

PK_INLINE void	glGenBufferIFN(bool doIt, GLuint *outBuffer)
{
	if (outBuffer && doIt)
	{
		if (*outBuffer == 0)
			glGenBuffers(1, outBuffer);
	}
	else
		glDelete1(Buffers, *outBuffer);
}

//----------------------------------------------------------------------------

PK_INLINE void	glGenBufferIFN(u32 doItMask, GLuint *outBuffer)
{
	glGenBufferIFN(doItMask != 0, outBuffer);
}

//----------------------------------------------------------------------------
#endif /* __SAMPLEGL_GL_UTILS_H__ */
