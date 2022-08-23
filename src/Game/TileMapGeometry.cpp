#include "stdafx.h"
#include "TileMapGeometry.h"
#include "Vertex.h"
#include "Camera.h"
#include "DrawPrimitive.h"
#include "Utility.h"
#include "TileMap.h"
#define DRAW_BOX_TEST 0
//-----------------------------------------------------------------------------
constexpr const char* vertexShaderSource = R"(
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
constexpr const char* fragmentShaderSource = R"(
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
	m_shaderProgram.CreateFromMemories(vertexShaderSource, fragmentShaderSource);
	m_shaderProgram.Bind();
	m_MatrixID = m_shaderProgram.GetUniformVariable("MVP");
	m_TextureID = m_shaderProgram.GetUniformVariable("Texture0");
	m_shaderProgram.SetUniform(m_TextureID, 0);

#if DRAW_BOX_TEST
	tex = gTextureManager->GetTexture2D("../data/textures/1mx1m.png");
	if (!tex) return false;
	constexpr Vertex_Pos3_TexCoord vertices[] = 
	{
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

	constexpr uint16_t indices[] = {
	   0, 1, 2,  0, 2, 3,
	   6, 5, 4,  7, 6, 4,
	   8, 9, 10,  8, 10, 11,
	   14, 13, 12,  15, 14, 12,
	   16, 17, 18,  16, 18, 19,
	   22, 21, 20,  23, 22, 20
	};
#else
	constexpr Vertex_Pos3_TexCoord vertices[] =
	{
		/* pos                     uvs */
		{ {-0.5f, -0.5f, 0.5f},  {0.0f, 0.0f} },
		{ { 0.5f, -0.5f, 0.5f},  {1.0f, 0.0f} },
		{ { 0.5f,  0.5f, 0.5f},  {1.0f, 1.0f} },
		{ {-0.5f,  0.5f, 0.5f},  {0.0f, 1.0f} },		
	};
	constexpr uint16_t indices[] = { 0, 1, 2, 2, 3, 0 };

#endif
	m_vertexBuf.Create(RenderResourceUsage::Static, Countof(vertices), sizeof(Vertex_Pos3_TexCoord), vertices);
	m_indexBuf.Create(RenderResourceUsage::Static, Countof(indices), sizeof(uint16_t), indices);

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
	m_shaderProgram.Destroy();
	m_vao.Destroy();
}
//-----------------------------------------------------------------------------
void TileMapGeometry::Draw(const Camera& camera, TilesCell* tiles)
{
#if DRAW_BOX_TEST
	{
		tex->Bind();
		m_shaderProgram.Bind();
		glm::mat4 ProjectionMatrix = camera.GetProjectionMatrix();
		glm::mat4 ViewMatrix = camera.GetViewMatrix();
		glm::mat4 RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -4));
		glm::mat4 ModelMatrix = TranslateMatrix * RotateMatrix;
		glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

		m_shaderProgram.SetUniform(m_MatrixID, MVP);
		m_vao.Draw();

		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, -2));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		m_shaderProgram.SetUniform(m_MatrixID, MVP);
		m_vao.Draw();

		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, -2));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		m_shaderProgram.SetUniform(m_MatrixID, MVP);
		m_vao.Draw();


		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4, 0, 0));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		m_shaderProgram.SetUniform(m_MatrixID, MVP);
		m_vao.Draw();

		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4, 0, 0));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		m_shaderProgram.SetUniform(m_MatrixID, MVP);
		m_vao.Draw();
	}
	drawPrimitive::DrawCubeWires(camera, glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(2.0f), glm::vec3(0.0f, glm::radians(45.0f), 0.0f), glm::vec4(0.4f, 1.0f, 0.4f, 0.7f), true);
#else
	m_shaderProgram.Bind();

	const int cameraPosX = floor(camera.GetPosition().x);
	const int cameraPosY = floor(camera.GetPosition().y);
	const int cameraPosZ = floor(camera.GetPosition().z);

	constexpr int viewDist = 80;

	// нужно по другому - нужно x,y,z  =равны камере - и увеличиваются до дистанции (ведь что влево, что вправо - логика одна, так зачем два шага цикла, если можно один и отзеркалить?

	for (int z = 0; z < SizeMapZ; z++)
	{
		for (int x = cameraPosX - viewDist; x < cameraPosX + viewDist; x++)
		{
			if (x < 0 || x >= SizeMap) continue;
			for (int y = cameraPosZ - viewDist; y < cameraPosZ + viewDist; y++)
			{
				if (y < 0 || y >= SizeMap) continue;
				if (!tiles->tiles[z][x][y] || !tiles->tiles[z][x][y]->tileTemplate) continue;

				{
					const float posX = (float)x;
					const float posZ = z - 1.0f;
					const float posY = (float)y;

					// задняя сторона
					if (y == 0 || !tiles->tiles[z][x][y - 1])
					{
						drawSide(tiles->tiles[z][x][y]->tileTemplate->textureLeft, camera, { posX, posZ, posY }, TileSide::Back);
					}

					// передняя сторона
					if (y == SizeMap - 1 || !tiles->tiles[z][x][y + 1])
					{
						drawSide(tiles->tiles[z][x][y]->tileTemplate->textureLeft, camera, { posX, posZ, posY }, TileSide::Forward);
					}

					// левая сторона
					if (x == 0 || !tiles->tiles[z][x - 1][y])
					{
						drawSide(tiles->tiles[z][x][y]->tileTemplate->textureLeft, camera, { posX, posZ, posY }, TileSide::Left);
					}

					// правая
					if (x == SizeMap-1 || !tiles->tiles[z][x + 1][y])
					{
						drawSide(tiles->tiles[z][x][y]->tileTemplate->textureLeft, camera, { posX, posZ, posY }, TileSide::Right);
					}

					// вверх
					if (cameraPosY > posZ)
					{
						if (z == SizeMapZ - 1 || !tiles->tiles[z + 1][x][y])
							drawSide(tiles->tiles[z][x][y]->tileTemplate->textureTop, camera, { posX, posZ, posY }, TileSide::Top);
					}
						
					// низ
					if (cameraPosY < posZ)
					{
						if (z == 0 || !tiles->tiles[z - 1][x][y])
							drawSide(tiles->tiles[z][x][y]->tileTemplate->textureTop, camera, { posX, posZ, posY }, TileSide::Bottom);
					}
				}
			}
		}
	}
#endif
}
//-----------------------------------------------------------------------------
void TileMapGeometry::drawSide(Texture2D* texture, const Camera& camera, const Vector3& pos, TileSide side)
{
	texture->Bind();

	glm::mat4 RotateMatrix;
	switch (side)
	{
	case TileSide::Top:
		RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case TileSide::Bottom:
		RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		break;
	case TileSide::Forward:
		RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case TileSide::Back:
		RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case TileSide::Left:
		RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	case TileSide::Right:
		RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		break;
	default:
		break;
	}	
	glm::mat4 TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(pos.x, pos.y, pos.z));
	glm::mat4 ModelMatrix = TranslateMatrix * RotateMatrix;
	glm::mat4 MVP = camera.GetProjectionMatrix() * camera.GetViewMatrix() * ModelMatrix;

	m_shaderProgram.SetUniform(m_MatrixID, MVP);
	m_vao.Draw();
}
//-----------------------------------------------------------------------------