#include "stdafx.h"
#include "RenderResource.h"
#include "Camera.h"
#include "DrawPrimitive.h"

Camera camera;

VertexArrayBuffer vao;
VertexBuffer vertexBuf;
IndexBuffer indexBuf;

struct vertex_t
{
	glm::vec3 position;
	glm::vec2 texCoord;
};

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
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	shaderProgram2.CreateFromMemories(g_sample_vs_src, g_sample_fs_src);
	shaderProgram2.Bind();
	MatrixID = shaderProgram2.GetUniformVariable("MVP");
	TextureID = shaderProgram2.GetUniformVariable("myTextureSampler");
	shaderProgram2.SetUniform(TextureID, 0);

	Texture2DLoaderInfo textureLoaderInfo;
	textureLoaderInfo.fileName = "../data/textures/1mx1m.png";
	texture.CreateFromFiles(textureLoaderInfo);

	vertex_t vertices[] = {
		/* pos                  uvs */
		{ {-1.0f, -1.0f, -1.0f},  {0.0f, 0.0f} },
		{ { 1.0f, -1.0f, -1.0f},  {1.0f, 0.0f} },
		{ { 1.0f,  1.0f, -1.0f},  {1.0f, 1.0f} },
		{ {-1.0f,  1.0f, -1.0f},  {0.0f, 1.0f} },

		{ {-1.0f, -1.0f,  1.0f},  {0.0f, 0.0f} },
		{ { 1.0f, -1.0f,  1.0f},  {1.0f, 0.0f} },
		{ { 1.0f,  1.0f,  1.0f},  {1.0f, 1.0f} },
		{ {-1.0f,  1.0f,  1.0f},  {0.0f, 1.0f} },

		{ {-1.0f, -1.0f, -1.0f},  {0.0f, 0.0f} },
		{ {-1.0f,  1.0f, -1.0f},  {1.0f, 0.0f} },
		{ {-1.0f,  1.0f,  1.0f},  {1.0f, 1.0f} },
		{ {-1.0f, -1.0f,  1.0f},  {0.0f, 1.0f} },

		{ { 1.0f, -1.0f, -1.0f},  {0.0f, 0.0f} },
		{ { 1.0f,  1.0f, -1.0f},  {1.0f, 0.0f} },
		{ { 1.0f,  1.0f,  1.0f},  {1.0f, 1.0f} },
		{ { 1.0f, -1.0f,  1.0f},  {0.0f, 1.0f} },

		{ {-1.0f, -1.0f, -1.0f},  {0.0f, 0.0f} },
		{ {-1.0f, -1.0f,  1.0f},  {1.0f, 0.0f} },
		{ { 1.0f, -1.0f,  1.0f},  {1.0f, 1.0f} },
		{ { 1.0f, -1.0f, -1.0f},  {0.0f, 1.0f} },
	
		{ {-1.0f,  1.0f, -1.0f},  {0.0f, 0.0f} },
		{ {-1.0f,  1.0f,  1.0f},  {1.0f, 0.0f} },
		{ { 1.0f,  1.0f,  1.0f},  {1.0f, 1.0f} },
		{ { 1.0f,  1.0f, -1.0f},  {0.0f, 1.0f} },
	};

	vertexBuf.Create(RenderResourceUsage::Static, 24, sizeof(vertex_t), vertices);


	uint16_t indices[] = {
	   0, 1, 2,  0, 2, 3,
	   6, 5, 4,  7, 6, 4,
	   8, 9, 10,  8, 10, 11,
	   14, 13, 12,  15, 14, 12,
	   16, 17, 18,  16, 18, 19,
	   22, 21, 20,  23, 22, 20
	};

	indexBuf.Create(RenderResourceUsage::Static, 36, sizeof(uint16_t), indices);
	
	std::vector<VertexAttribute> attribs =
	{
		{.size = 3, .type = GL_FLOAT, .normalized = false, .stride = sizeof(vertex_t), .pointer = (void*)offsetof(vertex_t, position)},
		{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(vertex_t), .pointer = (void*)offsetof(vertex_t, texCoord)},
	};
	vao.Create({ &vertexBuf }, &indexBuf, attribs);
	
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
	indexBuf.Destroy();
	shaderProgram2.Destroy();
	texture.Destroy();
	vao.Destroy();
}
//-----------------------------------------------------------------------------