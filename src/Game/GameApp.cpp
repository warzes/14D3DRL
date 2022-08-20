#include "stdafx.h"
//-----------------------------------------------------------------------------
extern int WindowWidth;
extern int WindowHeight;
//-----------------------------------------------------------------------------
const char* g_sample_vs_src = R"(
#version 330 core

uniform mat4 projectionMatrix;

in vec3 position;
in vec3 color;

out vec3 fragmentColor;

void main(void)
{
	gl_Position   = projectionMatrix * vec4(position, 1.0);
	fragmentColor = color;
}
)";

const char* g_sample_fs_src = R"(
#version 330 core

in vec3 fragmentColor;

out vec4 color;

void main(void)
{
	color = vec4(fragmentColor, 1.0);
}
)";

constexpr int MESH_VERTEX_COUNT = 3;
constexpr int VERTEX_SIZE = 6 * sizeof(float);

constexpr int VERTEX_POSITION_OFFSET = 0;
constexpr int VERTEX_COLOR_OFFSET = 3 * sizeof(float);

GLuint shaderProgram = 0, vertexShader = 0, fragmentShader = 0;
GLuint meshVAO = 0, meshVBO = 0;

float projectionMatrix[16];
GLint projectionMatrixLocation, positionLocation, colorLocation;

constexpr float triangleMesh[MESH_VERTEX_COUNT * 6] =
{
	-1.0f, -1.0f, -2.0f, /* цвет: */ 1.0f, 0.0f, 0.0f,
	 0.0f,  1.0f, -2.0f, /* цвет: */ 0.0f, 1.0f, 0.0f,
	 1.0f, -1.0f, -2.0f, /* цвет: */ 0.0f, 0.0f, 1.0f
};

// построение перспективной матрицы
void Matrix4Perspective(float* M, float fovy, float aspect, float znear, float zfar)
{
	const float f = 1 / tanf(fovy / 2);
	const float A = (zfar + znear) / (znear - zfar);
	const float B = (2 * zfar * znear) / (znear - zfar);

	M[0] = f / aspect; M[1] = 0; M[2] = 0; M[3] = 0;
	M[4] = 0;          M[5] = f; M[6] = 0; M[7] = 0;
	M[8] = 0;          M[9] = 0; M[10] = A; M[11] = B;
	M[12] = 0;          M[13] = 0; M[14] = -1; M[15] = 0;
}
//-----------------------------------------------------------------------------
bool GameAppInit()
{
	shaderProgram = glCreateProgram();
	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertexShader, 1, (const GLchar**)&g_sample_vs_src, nullptr);
	glCompileShader(vertexShader);
	if (ShaderStatus(vertexShader, GL_COMPILE_STATUS) != GL_TRUE)
		return false;

	glShaderSource(fragmentShader, 1, (const GLchar**)&g_sample_fs_src, nullptr);
	glCompileShader(fragmentShader);
	if (ShaderStatus(fragmentShader, GL_COMPILE_STATUS) != GL_TRUE)
		return false;

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	glLinkProgram(shaderProgram);
	if (ShaderProgramStatus(shaderProgram, GL_LINK_STATUS) != GL_TRUE)
		return false;

	glUseProgram(shaderProgram);

	Matrix4Perspective(projectionMatrix, 45.0f, (float)WindowWidth / (float)WindowHeight, 0.5f, 5.0f);

	projectionMatrixLocation = glGetUniformLocation(shaderProgram, "projectionMatrix");
	if (projectionMatrixLocation != -1)
		glUniformMatrix4fv(projectionMatrixLocation, 1, GL_TRUE, projectionMatrix);

	glValidateProgram(shaderProgram);
	if (ShaderProgramStatus(shaderProgram, GL_VALIDATE_STATUS) != GL_TRUE)
		return false;

	glGenVertexArrays(1, &meshVAO);
	glBindVertexArray(meshVAO);

	glGenBuffers(1, &meshVBO);
	glBindBuffer(GL_ARRAY_BUFFER, meshVBO);

	glBufferData(GL_ARRAY_BUFFER, MESH_VERTEX_COUNT * VERTEX_SIZE, triangleMesh, GL_STATIC_DRAW);

	positionLocation = glGetAttribLocation(shaderProgram, "position");
	if (positionLocation != -1)
	{
		glVertexAttribPointer(positionLocation, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const GLvoid*)VERTEX_POSITION_OFFSET);
		glEnableVertexAttribArray(positionLocation);
	}

	colorLocation = glGetAttribLocation(shaderProgram, "color");
	if (colorLocation != -1)
	{
		glVertexAttribPointer(colorLocation, 3, GL_FLOAT, GL_FALSE, VERTEX_SIZE, (const GLvoid*)VERTEX_COLOR_OFFSET);
		glEnableVertexAttribArray(colorLocation);
	}

	return true;
}
//-----------------------------------------------------------------------------
void GameAppUpdate()
{
}
//-----------------------------------------------------------------------------
void GameAppFrame()
{
	glUseProgram(shaderProgram);
	glBindVertexArray(meshVAO);
	glDrawArrays(GL_TRIANGLES, 0, MESH_VERTEX_COUNT);
}
//-----------------------------------------------------------------------------
void GameAppClose()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDeleteBuffers(1, &meshVBO);

	glBindVertexArray(0);
	glDeleteVertexArrays(1, &meshVAO);

	glUseProgram(0);
	glDeleteProgram(shaderProgram);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}
//-----------------------------------------------------------------------------