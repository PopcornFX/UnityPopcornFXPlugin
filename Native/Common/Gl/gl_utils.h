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

#define	GL_GLEXT_PROTOTYPES	1

#include <GL/glew.h>

#include "popcornfx.h"

#if	defined(PK_MACOSX)
#	include <OpenGL/gl.h>
#	include <OpenGl/glu.h>
#	include <GLUT/glut.h>
#else
#	include <GL/gl.h>
#endif

#if	!defined(PK_MACOSX)
#	include "printf_utils.h"
#endif

#ifndef	PK_BUILD_WITH_OPENGL3
#	define	PK_BUILD_WITH_OPENGL3	1
#endif

#if 1
#	define PK_ASSERT_GL_ERROR(__errorCode, __fmt, ...)		do { \
		const GLenum	errorCode = __errorCode; \
		if (errorCode != GL_NO_ERROR) \
		{ \
			PopcornFX::CLog::Log(PK_ERROR, "Error OpenGL: %s(%d) [%s]: " __fmt, __FILE__, __LINE__, glGetStrError(errorCode), ##__VA_ARGS__); \
			PK_ASSERT_MESSAGE(false, "Error OpenGL [%s]: " __fmt, glGetStrError(errorCode), ##__VA_ARGS__); \
		} \
	} while (0)
#else
#	define PK_ASSERT_GL_ERROR(...)
#endif

//----------------------------------------------------------------------------
// check only on debug mode

#ifdef	PK_DEBUG
#	define	GLERR \
	do { \
		const GLenum	e = glGetError(); \
		PK_ASSERT_GL_ERROR(e, "GLERR"); \
	} while (0)

#else
#	define	GLERR
#endif

//----------------------------------------------------------------------------
// always check errors:

#define	GL_ERROR_MSG(__fmt, ...)	PK_ASSERT_GL_ERROR(glGetError(), __fmt, ##__VA_ARGS__)

#define	GL_ERROR(__call) \
	do { \
		__call; \
		PK_ASSERT_GL_ERROR(glGetError(), "%s", #__call); \
	} while (0)

#define	GL_IF_ERROR(__call, __todo_if_error) \
	do { \
		__call; \
		const GLenum	e = glGetError(); \
		PK_ASSERT_GL_ERROR(e, "%s", #__call); \
		if (e != GL_NO_ERROR) \
		{ \
			__todo_if_error; \
		} \
	} while (0)

#define	GLERROR(__msg, __todo_if_error) \
	do { \
		const GLenum	e = glGetError(); \
		PK_ASSERT_GL_ERROR(e, "%s", __msg); \
		if (e != GL_NO_ERROR) \
		{ \
			__todo_if_error; \
		} \
	} while (0)


//----------------------------------------------------------------------------
// Delete an opengl resource safly

#define	glDelete(__name, __glid)		do { if (__glid != 0) { glDelete ## __name (__glid); __glid = 0; } } while (0)
#define	glDeleteN(__nb, __name, __glid)	do { if (__glid != 0) { glDelete ## __name (__nb, __glid); for (u32 i = 0; i < (__nb); ++i) (__glid)[i] = 0; } } while (0)
#define	glDelete1(__name, __glid)		do { if (__glid != 0) { glDelete ## __name (1, &(__glid)); __glid = 0; } } while (0)

//----------------------------------------------------------------------------

#define	glFlushErrors()		do { \
		GLenum	err; \
		while ((err = glGetError()) != GL_NO_ERROR) \
		{ \
			PK_ASSERT_GL_ERROR(err, "Error OpenGl flushed"); \
		} \
	} while (0)

#if (PK_BUILD_WITH_OPENGL3 != 0)
#	define PK_RUNTIME_GL4_SUPPORT glGLVersionGE(4, 0)
#	define PK_RUNTIME_GL3_SUPPORT glGLVersionGE(3, 0)
#else
#	define PK_RUNTIME_GL4_SUPPORT false
#	define PK_RUNTIME_GL3_SUPPORT false
#endif

#define PK_RUNTIME_GL1_4_SUPPORT GLEW_VERSION_1_4

//----------------------------------------------------------------------------

void			glClearErrors();
const char		*glGetStrError();
const char		*glGetFramebufferStatusStrError(GLenum glError);
const char		*glGetStrError(GLenum glError);

bool			glPrintShaderCompileInfo(GLuint shader);
bool			glPrintProgramLinkInfo(GLuint program);
bool			glCreateCompileShader(GLenum shaderType, char const ** codes, size_t codesCount, GLuint &outShader);
bool			glCreateCompileShader(GLenum shaderType, char const * code, GLuint &outShader);

PK_INLINE void	glEnableSrgbIFP() { glEnable(GL_FRAMEBUFFER_SRGB); }
PK_INLINE void	glDisableSrgbIFP() { glDisable(GL_FRAMEBUFFER_SRGB); }

void			glGenBufferIFN(bool doIt, GLuint *outBuffer);
void			glGenBufferIFN(PopcornFX::u32 doItMask, GLuint *outBuffer);

//----------------------------------------------------------------------------

bool			glHasExtension(const char *extensionString);
bool			glHasExtension(const char *extensionString, const char *messageIfNotSupported);
bool			glHasExtension(const char *extensionString, bool otherConditionToBeTrue, const char *messageIfNotSupported);

bool			glGLVersionGE(int maj, int min);

//----------------------------------------------------------------------------

bool			CheckShaderCompilation(GLuint shaderModule);
GLuint			CreateShaderProgramFromSources(const PopcornFX::CString &vertexShaderSource, const PopcornFX::CString &fragmentShaderSource);

//----------------------------------------------------------------------------
#endif /* __SAMPLEGL_GL_UTILS_H__ */
