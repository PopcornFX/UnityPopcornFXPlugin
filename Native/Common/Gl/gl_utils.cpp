//----------------------------------------------------------------------------
// This program is the property of Persistant Studios SARL.
//
// You may not redistribute it and/or modify it under any conditions
// without written permission from Persistant Studios SARL, unless
// otherwise stated in the latest Persistant Studios Code License.
//
// See the Persistant Studios Code License for further details.
//----------------------------------------------------------------------------

#include "precompiled.h"

#include <iostream>

#include "gl_utils.h"

#include <pk_kernel/include/kr_log.h>

//----------------------------------------------------------------------------

using namespace	PK_NAMESPACE;

#define	_GLGETERRSTR(__enum)		{ __enum, #__enum }

const char	*glGetFramebufferStatusStrError(GLenum glError)
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
		_GLGETERRSTR(GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER),
		_GLGETERRSTR(GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER),
		_GLGETERRSTR(GL_FRAMEBUFFER_UNSUPPORTED),
		_GLGETERRSTR(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE),
		_GLGETERRSTR(GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE),
		_GLGETERRSTR(GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS)
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

const char	*glGetStrError(GLenum glError)
{
	static const struct
	{
		size_t			e;
		char const *	str;
	} errs[] =
	{
		_GLGETERRSTR(GL_NO_ERROR),
		_GLGETERRSTR(GL_INVALID_ENUM),
		_GLGETERRSTR(GL_INVALID_VALUE),
		_GLGETERRSTR(GL_INVALID_OPERATION),
		_GLGETERRSTR(GL_INVALID_FRAMEBUFFER_OPERATION),
		_GLGETERRSTR(GL_STACK_OVERFLOW),
		_GLGETERRSTR(GL_STACK_UNDERFLOW),
		_GLGETERRSTR(GL_OUT_OF_MEMORY),
		_GLGETERRSTR(GL_TABLE_TOO_LARGE)
	};

	if (glError == GL_NO_ERROR)
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

const char	*glGetStrError()
{
	return glGetStrError(glGetError());
}

//----------------------------------------------------------------------------

void	glClearErrors()
{
	const u32	maxError = 0xFF;
	for (u32 i = 0; i < maxError && glGetError() != GL_NO_ERROR; ++i)
	{
		;
	}
}

//----------------------------------------------------------------------------

bool	glPrintShaderCompileInfo(GLuint shader)
{
	GLint	status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	GLERROR("Shader compile status", return false);

	int	logsize = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
	GLERROR("Shader compile log length", return false);

	if (logsize > 1)
	{
		char	*log = reinterpret_cast<char*>(malloc(logsize + 1));
		if (log != null)
		{
			glGetShaderInfoLog(shader, logsize, &logsize, log);
			GLERROR("Shader info log", return false);
			log[logsize] = '\0';	// some implementations do not add the terminating '\0'

			if (logsize > 0)
			{
				if (status == GL_TRUE)
				{
					CLog::Log(PK_INFO, "----- shader ok --------------------------------------------------\n");
					CLog::Log(PK_INFO, "%s\n", log);
				}
				else
				{
					int sourceSize = 0;
					glGetShaderiv(shader, GL_SHADER_SOURCE_LENGTH, &sourceSize);
					char	*source = reinterpret_cast<char*>(malloc(sourceSize + 1));
					glGetShaderSource(shader, sourceSize, &sourceSize, source);

					CLog::Log(PK_ERROR, "-- !! shader ERROR !! --------------------------------------------\n");
					CLog::Log(PK_ERROR, "%s\n", source);
					CLog::Log(PK_ERROR, "%s\n", log);
				}
				if (status == GL_TRUE)
					CLog::Log(PK_INFO, "------------------------------------------------------------------\n");
				else
					CLog::Log(PK_ERROR, "---!!-------------------------------------------------------------\n");
			}
			free(log);
		}
	}

	return status == GL_TRUE;
}

//----------------------------------------------------------------------------

bool	glPrintProgramLinkInfo(GLuint program)
{
	GLint	status;

	glGetProgramiv(program, GL_LINK_STATUS, &status);
	GLERROR("Program link status", return false);

	int	logsize = 0;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logsize);
	GLERROR("Program link log length", return false);

	if (logsize > 0)
	{
		char	*log = reinterpret_cast<char*>(malloc(logsize + 1));
		if (log != null)
		{
			glGetProgramInfoLog(program, logsize, &logsize, log);
			GLERROR("Program info log", return false);
			log[logsize] = '\0';	// some implementations do not add the terminating '\0'
			if (logsize > 0)
			{
				if (status == GL_TRUE)
					fprintf(stderr, "----- program ok -------------------------------------------------\n");
				else
					fprintf(stderr, "-- !! program ERROR !! -------------------------------------------\n");
				fprintf(stderr, "%s\n", log);
				if (status == GL_TRUE)
					fprintf(stderr, "------------------------------------------------------------------\n");
				else
					fprintf(stderr, "---!!-------------------------------------------------------------\n");
			}
			free(log);
		}
	}

	return status == GL_TRUE;
}

//----------------------------------------------------------------------------

bool	glCreateCompileShader(GLenum shaderType, char const * code, GLuint &outShader)
{
	const char		*codes[] = {code};
	return glCreateCompileShader(shaderType, codes, 1, outShader);
}

//----------------------------------------------------------------------------

bool	glCreateCompileShader(GLenum shaderType, char const **codes, size_t codesCount, GLuint &outShader)
{
	glGetError(); // reset Error

	GLuint		&shader = outShader;

	shader = glCreateShader(shaderType);
	GLERROR("Create shader", return false);

	glShaderSource(shader, checked_numcast<GLsizei>(codesCount), codes, NULL);
	GLERROR("Shader source", return false);

	glCompileShader(shader);
	GLERROR("Compile source", return false);

	return glPrintShaderCompileInfo(shader);
}

//----------------------------------------------------------------------------

void	glGenBufferIFN(bool doIt, GLuint *outBuffer)
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

void	glGenBufferIFN(u32 doItMask, GLuint *outBuffer)
{
	glGenBufferIFN(doItMask != 0, outBuffer);
}

//----------------------------------------------------------------------------

bool	glHasExtension(const char *extensionString)
{
	PK_ASSERT(extensionString != null);
	return glewIsExtensionSupported(extensionString) != 0;
}

//----------------------------------------------------------------------------

bool	glHasExtension(const char *extensionString, const char *messageIfNotSupported)
{
	return glHasExtension(extensionString, true, messageIfNotSupported);
}

//----------------------------------------------------------------------------

bool	glHasExtension(const char *extensionString, bool otherConditionToBeTrue, const char *messageIfNotSupported)
{
	if (glHasExtension(extensionString) && otherConditionToBeTrue)
	{
#ifdef PK_DEBUG
		CLog::Log(PK_INFO, "OK, OpenGL %s supported for %s", extensionString, messageIfNotSupported);
#endif // PK_DEBUG
		return true;
	}
	CLog::Log(PK_ERROR, "ERROR, OpenGL %s not supported for %s", extensionString, messageIfNotSupported);
	return false;
}

//----------------------------------------------------------------------------

bool	glGLVersionGE(int maj, int min)
{
	static int	glmaj = -1;
	static int	glmin = -1;
	if (glmaj <= 0)
	{
		glGetIntegerv(GL_MAJOR_VERSION, &glmaj);
		glGetIntegerv(GL_MINOR_VERSION, &glmin);
		glClearErrors();
		//printf("glGLVersionGE: OpenGL version %d.%d\n", glmaj, glmin);
		if (glmaj <= 0) // GL_MAJOR_VERSION only since gl 3.0
		{
			glmaj = 2;
			glmin = 1;
		}
	}
	if (glmaj > maj)
		return true;
	if (glmaj == maj)
		return glmin >= min;
	return false;
}

//----------------------------------------------------------------------------
//
//	Shader program compilation
//
//----------------------------------------------------------------------------

bool	CheckShaderCompilation(GLuint shaderModule)
{
	GLint	isCompiled = 0;
	glGetShaderiv(shaderModule, GL_COMPILE_STATUS, &isCompiled);
	if (isCompiled == GL_FALSE)
	{
		GLint	maxLength = 0;
		glGetShaderiv(shaderModule, GL_INFO_LOG_LENGTH, &maxLength);
		TArray<GLchar>	infoLog(maxLength);
		glGetShaderInfoLog(shaderModule, maxLength, &maxLength, &infoLog[0]);
		glDeleteShader(shaderModule);
		CLog::Log(PK_ERROR, "Shader compilation error: '%s'", infoLog.RawDataPointer());
		return false;
	}
	return true;
}

//----------------------------------------------------------------------------

GLuint	CreateShaderProgramFromSources(const CString &vertexShaderSource, const CString &fragmentShaderSource)
{
	GLuint		vertexShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint		fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	GLERR;

	const char	*_vertexShaderSource = vertexShaderSource.Data();
	const char	*_fragmentShaderSource = fragmentShaderSource.Data();

	glShaderSource(vertexShader, 1, &_vertexShaderSource, null);
	glCompileShader(vertexShader);
	GLERR;

	if (!CheckShaderCompilation(vertexShader))
		return 0;

	glShaderSource(fragmentShader, 1, &_fragmentShaderSource, null);
	glCompileShader(fragmentShader);
	GLERR;

	if (!CheckShaderCompilation(fragmentShader))
		return 0;

	GLuint		currentShaderProgram = glCreateProgram();

	glAttachShader(currentShaderProgram, vertexShader);
	glAttachShader(currentShaderProgram, fragmentShader);

	glLinkProgram(currentShaderProgram);
	GLERR;

	GLint isLinked = 0;
	glGetProgramiv(currentShaderProgram, GL_LINK_STATUS, (int *)&isLinked);
	if (isLinked == GL_FALSE)
	{
		GLint maxLength = 0;
		glGetProgramiv(currentShaderProgram, GL_INFO_LOG_LENGTH, &maxLength);
		TArray<GLchar>	infoLog(maxLength);
		glGetProgramInfoLog(currentShaderProgram, maxLength, &maxLength, &infoLog[0]);
		glDeleteProgram(currentShaderProgram);
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
		CLog::Log(PK_ERROR, "Shader linkage error: '%s'", infoLog.RawDataPointer());
		return 0;
	}

	glDetachShader(currentShaderProgram, vertexShader);
	glDetachShader(currentShaderProgram, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
	return currentShaderProgram;
}

//----------------------------------------------------------------------------
