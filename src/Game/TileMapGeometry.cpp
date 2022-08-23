#include "stdafx.h"
#include "TileMapGeometry.h"
#include "Vertex.h"
#include "TextureManager.h"
#include "Camera.h"
#include "DrawPrimitive.h"
//-----------------------------------------------------------------------------
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
const char* g_sample_fs_src2 = R"(
#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D Texture0;

void main()
{
	color = texture( Texture0, UV );
}
)";
//-----------------------------------------------------------------------------
bool TileMapGeometry::Init()
{
	shaderProgram2.CreateFromMemories(g_sample_vs_src, g_sample_fs_src2);
	shaderProgram2.Bind();
	MatrixID = shaderProgram2.GetUniformVariable("MVP");
	TextureID = shaderProgram2.GetUniformVariable("Texture0");
	shaderProgram2.SetUniform(TextureID, 0);

	tex = gTextureManager->GetTexture2D("../data/textures/1.png");
	if (!tex) return false;

	Vertex_Pos3_TexCoord vertices[] = {
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

	m_vertexBuf.Create(RenderResourceUsage::Static, 24, sizeof(Vertex_Pos3_TexCoord), vertices);

	uint16_t indices[] = {
	   0, 1, 2,  0, 2, 3,
	   6, 5, 4,  7, 6, 4,
	   8, 9, 10,  8, 10, 11,
	   14, 13, 12,  15, 14, 12,
	   16, 17, 18,  16, 18, 19,
	   22, 21, 20,  23, 22, 20
	};

	m_indexBuf.Create(RenderResourceUsage::Static, 36, sizeof(uint16_t), indices);

	std::vector<VertexAttribute> attribs =
	{
		{.size = 3, .type = GL_FLOAT, .normalized = false, .stride = sizeof(Vertex_Pos3_TexCoord), .pointer = (void*)offsetof(Vertex_Pos3_TexCoord, position)},
		{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(Vertex_Pos3_TexCoord), .pointer = (void*)offsetof(Vertex_Pos3_TexCoord, texCoord)},
	};
	m_vao.Create({ &m_vertexBuf }, &m_indexBuf, attribs);

	return true;
}
//-----------------------------------------------------------------------------
void TileMapGeometry::Close()
{
	m_vertexBuf.Destroy();
	m_indexBuf.Destroy();
	shaderProgram2.Destroy();
	m_vao.Destroy();
}
//-----------------------------------------------------------------------------
void TileMapGeometry::Draw(const Camera& camera)
{
	glm::mat4 ProjectionMatrix = camera.GetProjectionMatrix();
	glm::mat4 ViewMatrix = camera.GetViewMatrix();
	glm::mat4 RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -4));
	glm::mat4 ModelMatrix = TranslateMatrix * RotateMatrix;
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	shaderProgram2.Bind();
	shaderProgram2.SetUniform(MatrixID, MVP);
	tex->Bind();
	m_vao.Draw();

	{
		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, -2));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		shaderProgram2.SetUniform(MatrixID, MVP);
		m_vao.Draw();

		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, -2));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		shaderProgram2.SetUniform(MatrixID, MVP);
		m_vao.Draw();


		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4, 0, 0));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		shaderProgram2.SetUniform(MatrixID, MVP);
		m_vao.Draw();

		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4, 0, 0));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		shaderProgram2.SetUniform(MatrixID, MVP);
		m_vao.Draw();
	}

	drawPrimitive::DrawCubeWires(camera, glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(2.0f), glm::vec3(0.0f, glm::radians(45.0f), 0.0f), glm::vec4(0.4f, 1.0f, 0.4f, 0.7f), true);
}
//-----------------------------------------------------------------------------