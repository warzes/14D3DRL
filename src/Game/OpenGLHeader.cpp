#include "stdafx.h"
#include "OpenGLHeader.h"
#include "Log.h"
//-----------------------------------------------------------------------------
// Texture
PFNGLACTIVETEXTUREPROC glActiveTexture = nullptr;
PFNGLGENERATEMIPMAPPROC glGenerateMipmap = nullptr;
// VAO
PFNGLGENVERTEXARRAYSPROC    glGenVertexArrays = nullptr;
PFNGLDELETEVERTEXARRAYSPROC glDeleteVertexArrays = nullptr;
PFNGLBINDVERTEXARRAYPROC    glBindVertexArray = nullptr;
// VBO
PFNGLGENBUFFERSPROC    glGenBuffers = nullptr;
PFNGLDELETEBUFFERSPROC glDeleteBuffers = nullptr;
PFNGLBINDBUFFERPROC    glBindBuffer = nullptr;
PFNGLBUFFERDATAPROC    glBufferData = nullptr;
PFNGLBUFFERSUBDATAPROC glBufferSubData = nullptr;
// Shaders
PFNGLCREATEPROGRAMPROC     glCreateProgram = nullptr;
PFNGLDELETEPROGRAMPROC     glDeleteProgram = nullptr;
PFNGLLINKPROGRAMPROC       glLinkProgram = nullptr;
PFNGLVALIDATEPROGRAMPROC   glValidateProgram = nullptr;
PFNGLUSEPROGRAMPROC        glUseProgram = nullptr;
PFNGLGETPROGRAMIVPROC      glGetProgramiv = nullptr;
PFNGLGETPROGRAMINFOLOGPROC glGetProgramInfoLog = nullptr;
PFNGLCREATESHADERPROC      glCreateShader = nullptr;
PFNGLDELETESHADERPROC      glDeleteShader = nullptr;
PFNGLSHADERSOURCEPROC      glShaderSource = nullptr;
PFNGLCOMPILESHADERPROC     glCompileShader = nullptr;
PFNGLATTACHSHADERPROC      glAttachShader = nullptr;
PFNGLDETACHSHADERPROC      glDetachShader = nullptr;
PFNGLGETSHADERIVPROC       glGetShaderiv = nullptr;
PFNGLGETSHADERINFOLOGPROC  glGetShaderInfoLog = nullptr;
// Shaders attributes
PFNGLGETATTRIBLOCATIONPROC        glGetAttribLocation = nullptr;
PFNGLVERTEXATTRIBPOINTERPROC      glVertexAttribPointer = nullptr;
PFNGLENABLEVERTEXATTRIBARRAYPROC  glEnableVertexAttribArray = nullptr;
PFNGLDISABLEVERTEXATTRIBARRAYPROC glDisableVertexAttribArray = nullptr;
// Shaders uniforms
PFNGLGETUNIFORMLOCATIONPROC glGetUniformLocation = nullptr;
PFNGLUNIFORM1IPROC          glUniform1i = nullptr;
PFNGLUNIFORM1FPROC          glUniform1f = nullptr;
PFNGLUNIFORM2FVPROC         glUniform2fv = nullptr;
PFNGLUNIFORM3FPROC          glUniform3f = nullptr;
PFNGLUNIFORM3FVPROC         glUniform3fv = nullptr;
PFNGLUNIFORM4FVPROC         glUniform4fv = nullptr;
PFNGLUNIFORMMATRIX3FVPROC   glUniformMatrix3fv = nullptr;
PFNGLUNIFORMMATRIX4FVPROC   glUniformMatrix4fv = nullptr;
// Draw command
PFNGLDRAWELEMENTSBASEVERTEXPROC glDrawElementsBaseVertex = nullptr;
//-----------------------------------------------------------------------------
#define OPENGL_GET_PROC(p,n) \
	n = (p)(void*)wglGetProcAddress(#n); \
	if (nullptr == n) \
	{ \
		std::string err = "Loading extension '" + std::string(#n) + "' fail."; \
		LogFatal(err.c_str()); \
		return false; \
	}
//-----------------------------------------------------------------------------
bool OpenGLInitExtensions()
{
	// Texture
	OPENGL_GET_PROC(PFNGLACTIVETEXTUREPROC, glActiveTexture);
	OPENGL_GET_PROC(PFNGLGENERATEMIPMAPPROC, glGenerateMipmap);
	// VAO
	OPENGL_GET_PROC(PFNGLGENVERTEXARRAYSPROC, glGenVertexArrays);
	OPENGL_GET_PROC(PFNGLDELETEVERTEXARRAYSPROC, glDeleteVertexArrays);
	OPENGL_GET_PROC(PFNGLBINDVERTEXARRAYPROC, glBindVertexArray);
	// VBO
	OPENGL_GET_PROC(PFNGLGENBUFFERSPROC, glGenBuffers);
	OPENGL_GET_PROC(PFNGLDELETEBUFFERSPROC, glDeleteBuffers);
	OPENGL_GET_PROC(PFNGLBINDBUFFERPROC, glBindBuffer);
	OPENGL_GET_PROC(PFNGLBUFFERDATAPROC, glBufferData);
	OPENGL_GET_PROC(PFNGLBUFFERSUBDATAPROC, glBufferSubData);
	// Shaders
	OPENGL_GET_PROC(PFNGLCREATEPROGRAMPROC, glCreateProgram);
	OPENGL_GET_PROC(PFNGLDELETEPROGRAMPROC, glDeleteProgram);
	OPENGL_GET_PROC(PFNGLLINKPROGRAMPROC, glLinkProgram);
	OPENGL_GET_PROC(PFNGLVALIDATEPROGRAMPROC, glValidateProgram);
	OPENGL_GET_PROC(PFNGLUSEPROGRAMPROC, glUseProgram);
	OPENGL_GET_PROC(PFNGLGETPROGRAMIVPROC, glGetProgramiv);
	OPENGL_GET_PROC(PFNGLGETPROGRAMINFOLOGPROC, glGetProgramInfoLog);
	OPENGL_GET_PROC(PFNGLCREATESHADERPROC, glCreateShader);
	OPENGL_GET_PROC(PFNGLDELETESHADERPROC, glDeleteShader);
	OPENGL_GET_PROC(PFNGLSHADERSOURCEPROC, glShaderSource);
	OPENGL_GET_PROC(PFNGLCOMPILESHADERPROC, glCompileShader);
	OPENGL_GET_PROC(PFNGLATTACHSHADERPROC, glAttachShader);
	OPENGL_GET_PROC(PFNGLDETACHSHADERPROC, glDetachShader);
	OPENGL_GET_PROC(PFNGLGETSHADERIVPROC, glGetShaderiv);
	OPENGL_GET_PROC(PFNGLGETSHADERINFOLOGPROC, glGetShaderInfoLog);
	// Shaders attributes
	OPENGL_GET_PROC(PFNGLGETATTRIBLOCATIONPROC, glGetAttribLocation);
	OPENGL_GET_PROC(PFNGLVERTEXATTRIBPOINTERPROC, glVertexAttribPointer);
	OPENGL_GET_PROC(PFNGLENABLEVERTEXATTRIBARRAYPROC, glEnableVertexAttribArray);
	OPENGL_GET_PROC(PFNGLDISABLEVERTEXATTRIBARRAYPROC, glDisableVertexAttribArray);
	// Shaders uniforms
	OPENGL_GET_PROC(PFNGLGETUNIFORMLOCATIONPROC, glGetUniformLocation);
	OPENGL_GET_PROC(PFNGLUNIFORM1IPROC, glUniform1i);
	OPENGL_GET_PROC(PFNGLUNIFORM1FPROC, glUniform1f);
	OPENGL_GET_PROC(PFNGLUNIFORM2FVPROC, glUniform2fv);
	OPENGL_GET_PROC(PFNGLUNIFORM3FPROC, glUniform3f);
	OPENGL_GET_PROC(PFNGLUNIFORM3FVPROC, glUniform3fv);
	OPENGL_GET_PROC(PFNGLUNIFORM4FVPROC, glUniform4fv);
	OPENGL_GET_PROC(PFNGLUNIFORMMATRIX3FVPROC, glUniformMatrix3fv);
	OPENGL_GET_PROC(PFNGLUNIFORMMATRIX4FVPROC, glUniformMatrix4fv);
	// Draw command
	OPENGL_GET_PROC(PFNGLDRAWELEMENTSBASEVERTEXPROC, glDrawElementsBaseVertex);

	return true;
}
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