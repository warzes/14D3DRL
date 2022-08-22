#include "stdafx.h"
#include "OpenGLHeader.h"
#include "Log.h"
//-----------------------------------------------------------------------------
GLint ShaderProgramStatus(GLuint program, GLenum param)
{
	GLint status, length;
	GLchar buffer[1024];

	glGetProgramiv(program, param, &status);
	if (status != GL_TRUE)
	{
		glGetProgramInfoLog(program, 1024, &length, buffer);

		std::string err = "Shader program: " + std::string((const char*)buffer);
		LogFatal(err.c_str());
	}

	return status;
}
//-----------------------------------------------------------------------------
GLint ShaderStatus(GLuint shader, GLenum param)
{
	GLint status, length;
	GLchar buffer[1024];

	glGetShaderiv(shader, param, &status);

	if (status != GL_TRUE)
	{
		glGetShaderInfoLog(shader, 1024, &length, buffer);
		std::string err = "Shader: " + std::string((const char*)buffer);
		LogFatal(err.c_str());
	}
	return status;
}
//-----------------------------------------------------------------------------