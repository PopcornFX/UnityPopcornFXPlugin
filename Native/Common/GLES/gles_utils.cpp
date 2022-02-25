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

#include "gles_utils.h"

#include <cstdlib>
//----------------------------------------------------------------------------

bool	glPrintShaderCompileInfo(GLuint shader)
{
	GLint	status;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	GLERROR("Shader compile status", return false);

	int	logsize = 0;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logsize);
	GLERROR("Shader compile log length", return false);
	if (logsize > 0)
	{
		char	*log = reinterpret_cast<char*>(malloc(logsize + 1));
		if (log != null)
		{
			glGetShaderInfoLog(shader, logsize, &logsize, log);
			GLERROR("Shader info log", return false);
			log[logsize] = '\0';	// some implementations do not add the terminating '\0'

			if (status == GL_TRUE)
				fprintf(stderr, "----- shader ok --------------------------------------------------\n");
			else
				fprintf(stderr, "-- !! shader ERROR !! --------------------------------------------\n");
			fprintf(stderr, "%s\n", log);
			if (status == GL_TRUE)
				fprintf(stderr, "------------------------------------------------------------------\n");
			else
				fprintf(stderr, "---!!-------------------------------------------------------------\n");

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

			if (status == GL_TRUE)
				fprintf(stderr, "----- program ok -------------------------------------------------\n");
			else
				fprintf(stderr, "-- !! program ERROR !! -------------------------------------------\n");
			fprintf(stderr, "%s\n", log);
			if (status == GL_TRUE)
				fprintf(stderr, "------------------------------------------------------------------\n");
			else
				fprintf(stderr, "---!!-------------------------------------------------------------\n");

			free(log);
		}
	}

	return status == GL_TRUE;
}

//----------------------------------------------------------------------------

bool	glCreateCompileShader(GLenum shaderType, char const * code, GLuint &outShader)
{
	const char	*codes[] = {code};
	return glCreateCompileShader(shaderType, codes, 1, outShader);
}

//----------------------------------------------------------------------------

bool	glCreateCompileShader(GLenum shaderType, char const **codes, size_t codesCount, GLuint &outShader)
{
	glGetError(); // reset Error

	GLuint	&shader = outShader;

	shader = glCreateShader(shaderType);
	GLERROR("Create shader", return false);

	glShaderSource(shader, codesCount, codes, NULL);
	GLERROR("Shader source", return false);

	glCompileShader(shader);
	GLERROR("Compile source", return false);

	return glPrintShaderCompileInfo(shader);
}

//----------------------------------------------------------------------------

bool	glHasExtension(const char *nameStrings)
{
	const char *ext = (const char *)glGetString(GL_EXTENSIONS);

	return strstr(ext, nameStrings) != NULL;
}

//----------------------------------------------------------------------------

void	glEnableSrgbIFP()
{
	if (false)//glHasExtension("GL_EXT_sRGB_write_control") || glHasExtension("_framebuffer_sRGB"))
	{
		// FRAMEBUFFER_SRGB_EXT 0x8DB9
		glEnable(0x8DB9);
	}
}

//----------------------------------------------------------------------------

void	glDisableSrgbIFP()
{
	if (false) //glHasExtension("GL_EXT_sRGB_write_control") || glHasExtension("_framebuffer_sRGB"))
	{
		// FRAMEBUFFER_SRGB_EXT 0x8DB9
		glDisable(0x8DB9);
	}
}

//----------------------------------------------------------------------------

GLint	glSrgbTexFormatIFP()
{
	// FIXME : See why this won't work
	if (false)//glHasExtension("GL_EXT_sRGB"))
	{
		// SRGB8_ALPHA8_EXT 0x8C43
		// SRGB_ALPHA_EXT 0x8C42
		return 0x8C42;
	}
	return GL_RGBA;
}

//----------------------------------------------------------------------------
