#include "stdafx.h"
#include "RenderResource.h"
#include "Camera.h"
#include "DrawPrimitive.h"

Camera camera;

VertexArrayBuffer vao;
VertexBuffer vertexBuf;
VertexBuffer uvBuf;

Texture2D texture;

// Embedded vertex shader source.
const char* g_sample_vs_src = R"(
#version 330 core

layout(location = 0) in vec3 vertexPosition_modelspace;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

uniform mat4 MVP;

void main()
{
	gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
	UV = vertexUV;
}
)";

// Embedded fragment shader source.
const char* g_sample_fs_src = R"(
#version 330 core

in vec2 UV;

out vec3 color;

uniform sampler2D myTextureSampler;

void main()
{
	color = texture( myTextureSampler, UV ).rgb;
}
)";

ShaderProgram shaderProgram2;
UniformVariable MatrixID;
UniformVariable TextureID;
//-----------------------------------------------------------------------------
bool GameAppInit()
{
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	shaderProgram2.CreateFromMemories(g_sample_vs_src, g_sample_fs_src);
	shaderProgram2.Bind();
	MatrixID = shaderProgram2.GetUniformVariable("MVP");
	TextureID = shaderProgram2.GetUniformVariable("myTextureSampler");
	shaderProgram2.SetUniform(TextureID, 0);

	Texture2DLoaderInfo textureLoaderInfo;
	textureLoaderInfo.fileName = "../data/textures/1mx1m.png";
	texture.CreateFromFiles(textureLoaderInfo);

	static const GLfloat g_vertex_buffer_data[] = {
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		-1.0f,-1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		-1.0f,-1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f,-1.0f,
		 1.0f,-1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f,-1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f,-1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f, 1.0f, 1.0f,
		-1.0f, 1.0f, 1.0f,
		 1.0f,-1.0f, 1.0f
	};

	vertexBuf.Create(RenderResourceUsage::Static, 36, 3 * sizeof(float), g_vertex_buffer_data);

	static const GLfloat g_uv_buffer_data[] = {
		0.000059f, 0.000004f,
		0.000103f, 0.336048f,
		0.335973f, 0.335903f,
		1.000023f, 0.000013f,
		0.667979f, 0.335851f,
		0.999958f, 0.336064f,
		0.667979f, 0.335851f,
		0.336024f, 0.671877f,
		0.667969f, 0.671889f,
		1.000023f, 0.000013f,
		0.668104f, 0.000013f,
		0.667979f, 0.335851f,
		0.000059f, 0.000004f,
		0.335973f, 0.335903f,
		0.336098f, 0.000071f,
		0.667979f, 0.335851f,
		0.335973f, 0.335903f,
		0.336024f, 0.671877f,
		1.000004f, 0.671847f,
		0.999958f, 0.336064f,
		0.667979f, 0.335851f,
		0.668104f, 0.000013f,
		0.335973f, 0.335903f,
		0.667979f, 0.335851f,
		0.335973f, 0.335903f,
		0.668104f, 0.000013f,
		0.336098f, 0.000071f,
		0.000103f, 0.336048f,
		0.000004f, 0.671870f,
		0.336024f, 0.671877f,
		0.000103f, 0.336048f,
		0.336024f, 0.671877f,
		0.335973f, 0.335903f,
		0.667969f, 0.671889f,
		1.000004f, 0.671847f,
		0.667979f, 0.335851f
	};

	uvBuf.Create(RenderResourceUsage::Static, 36, 2 * sizeof(float), g_uv_buffer_data);

	std::vector<VertexAttribute> attribs =
	{
	{.size = 3, .type = GL_FLOAT, .normalized = false, .stride = 0, .pointer = (void*)0},
	{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = 0, .pointer = (void*)0},
	};
	vao.Create({ &vertexBuf, &uvBuf }, nullptr, attribs);
	
	return true;
}
//-----------------------------------------------------------------------------
void GameAppUpdate()
{
	camera.SimpleMove(0.01f);
}
//-----------------------------------------------------------------------------
void GameAppFrame()
{
	glm::mat4 ProjectionMatrix = camera.GetProjectionMatrix();
	glm::mat4 ViewMatrix = camera.GetViewMatrix();
	glm::mat4 RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -2));
	glm::mat4 ModelMatrix = TranslateMatrix * RotateMatrix;
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	shaderProgram2.Bind();
	shaderProgram2.SetUniform(MatrixID, MVP);
	texture.Bind();
	vao.Draw();
	drawPrimitive::DrawCubeWires(camera, glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(2.0f), glm::vec3(0.0f, glm::radians(45.0f),0.0f), glm::vec4(0.4f, 1.0f, 0.4f, 0.7f), true);
}
//-----------------------------------------------------------------------------
void GameAppClose()
{
	vertexBuf.Destroy();
	uvBuf.Destroy();
	shaderProgram2.Destroy();
	texture.Destroy();
	vao.Destroy();
}
//-----------------------------------------------------------------------------