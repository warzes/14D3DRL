#include "stdafx.h"
#include "TileMapGeometry.h"
#include "Vertex.h"
#include "Camera.h"
#include "DrawPrimitive.h"
#include "Utility.h"
#include "TileMap.h"
#define DRAW_BOX_TEST 0
#define DRAW_COMMAND 0
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

#if DRAW_COMMAND
struct CommandDrawTileInfo
{
	CommandDrawTileInfo() = default;
	CommandDrawTileInfo(const CommandDrawTileInfo&) = default;
	CommandDrawTileInfo& operator=(const CommandDrawTileInfo& c) = default;

	Texture2D* texture;
	glm::vec3 pos;
	TileSide side;

	float distance = 0.0f;
};
std::vector<CommandDrawTileInfo> gCommandDrawTileInfo;
int numCommandDrawTileInfo = 0;
#endif
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

	const glm::mat4 vp = camera.GetProjectionMatrix() * camera.GetViewMatrix();

	SimpleFrustum frustum;
	frustum.Extract(camera.GetProjectionMatrix(), camera.GetViewMatrix());

#if DRAW_COMMAND
	numCommandDrawTileInfo = 0;
#endif

#if 0 // получилось медленее
	// новый способ рисовани€

	constexpr int viewDist = 40;

	for (int z = 0; z < SizeMapZ; z++)
	{
		drawTile(frustum, cameraPosX, cameraPosZ, z, tiles, cameraPosY, vp);

		// 1 р€д
		for (int x = cameraPosX - 1; x <= cameraPosX + 1; x++)
		{
			drawTile(frustum, x, cameraPosZ - 1, z, tiles, cameraPosY, vp);
			drawTile(frustum, x, cameraPosZ + 1, z, tiles, cameraPosY, vp);
		}
		drawTile(frustum, cameraPosX - 1, cameraPosZ, z, tiles, cameraPosY, vp);
		drawTile(frustum, cameraPosX + 1, cameraPosZ, z, tiles, cameraPosY, vp);

		for (int i = 2; i < viewDist; i++)
		{
			for (int x = cameraPosX - i; x <= cameraPosX + i; x++)
			{
				drawTile(frustum, x, cameraPosZ - i, z, tiles, cameraPosY, vp);
				drawTile(frustum, x, cameraPosZ + i, z, tiles, cameraPosY, vp);
			}
			for (int y = cameraPosZ - i + 1; y <= cameraPosZ + i - 1; y++)
			{
				drawTile(frustum, cameraPosX - i, y, z, tiles, cameraPosY, vp);
				drawTile(frustum, cameraPosX + i, y, z, tiles, cameraPosY, vp);
			}
		}
	}
#else

	constexpr int viewDist = 60;
	for (int z = 0; z < SizeMapZ; z++)
	{
		for (int x = cameraPosX - viewDist; x < cameraPosX + viewDist; x++)
		{
			if (x < 0 || x >= SizeMap) continue;
			for (int y = cameraPosZ - viewDist; y < cameraPosZ + viewDist; y++)
			{
				if (y < 0 || y >= SizeMap) continue;
				drawTile(frustum, x, y, z, tiles, cameraPosY, vp, camera);
			}
		}
	}

#if DRAW_COMMAND
	struct sort_class_x
	{
		bool operator() (CommandDrawTileInfo i, CommandDrawTileInfo j)
		{
			return (i.distance < j.distance);
		}
	} sort_objectX;
	std::sort(gCommandDrawTileInfo.begin(), gCommandDrawTileInfo.begin()+ numCommandDrawTileInfo-1, sort_objectX);

	for(int ii = 0; ii < numCommandDrawTileInfo; ii++)
	{
		auto& cmd = gCommandDrawTileInfo[ii];
		const glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), cmd.pos);
		drawSide(cmd.texture, vp, translateMatrix, cmd.side);
	}
#endif
#endif
#endif
}
//-----------------------------------------------------------------------------
void TileMapGeometry::drawTile(const SimpleFrustum& frustum, int x, int y, int z, TilesCell* tiles, int cameraPosY, const glm::mat4& vp, const Camera& camera)
{
	if (x < 0 || x >= SizeMap || y < 0 || y >= SizeMap || z < 0 || z >= SizeMapZ) return;
	if (!tiles->tiles[z][x][y]) return;

	const Vector3 fpos = { (float)x, z - 1.0f, (float)y };
	const Vector3 min = { fpos.x - 0.5f, fpos.y - 0.5f, fpos.z - 0.5f };
	const Vector3 max = { fpos.x + 0.5f, fpos.y + 0.5f, fpos.z + 0.5f };
	if (!frustum.AABBoxIn(min, max)) return;

#if DRAW_COMMAND
	CommandDrawTileInfo command;
	command.pos = { fpos.x, fpos.y, fpos.z };
	command.distance = glm::distance(camera.GetPosition(), command.pos);

	// задн€€ сторона
	if (y == 0 || !tiles->tiles[z][x][y - 1])
	{
		command.texture = tiles->tiles[z][x][y]->tileTemplate->textureBack;
		command.side = TileSide::Back;

		if (numCommandDrawTileInfo == gCommandDrawTileInfo.size())
			gCommandDrawTileInfo.push_back(command);
		else
			gCommandDrawTileInfo[numCommandDrawTileInfo] = command;
		numCommandDrawTileInfo++;

	}
	// передн€€ сторона
	if (y == SizeMap - 1 || !tiles->tiles[z][x][y + 1])
	{
		command.texture = tiles->tiles[z][x][y]->tileTemplate->textureForward;
		command.side = TileSide::Forward;

		if (numCommandDrawTileInfo == gCommandDrawTileInfo.size())
			gCommandDrawTileInfo.push_back(command);
		else
			gCommandDrawTileInfo[numCommandDrawTileInfo] = command;
		numCommandDrawTileInfo++;
	}
	// лева€ сторона
	if (x == 0 || !tiles->tiles[z][x - 1][y])
	{
		command.texture = tiles->tiles[z][x][y]->tileTemplate->textureLeft;
		command.side = TileSide::Left;

		if (numCommandDrawTileInfo == gCommandDrawTileInfo.size())
			gCommandDrawTileInfo.push_back(command);
		else
			gCommandDrawTileInfo[numCommandDrawTileInfo] = command;
		numCommandDrawTileInfo++;
	}
	// права€
	if (x == SizeMap - 1 || !tiles->tiles[z][x + 1][y])
	{
		command.texture = tiles->tiles[z][x][y]->tileTemplate->textureRight;
		command.side = TileSide::Right;

		if (numCommandDrawTileInfo == gCommandDrawTileInfo.size())
			gCommandDrawTileInfo.push_back(command);
		else
			gCommandDrawTileInfo[numCommandDrawTileInfo] = command;
		numCommandDrawTileInfo++;
	}
	// вверх
	if (cameraPosY > fpos.y)
	{
		if (z == SizeMapZ - 1 || !tiles->tiles[z + 1][x][y])
		{
			command.texture = tiles->tiles[z][x][y]->tileTemplate->textureTop;
			command.side = TileSide::Top;

			if (numCommandDrawTileInfo == gCommandDrawTileInfo.size())
				gCommandDrawTileInfo.push_back(command);
			else
				gCommandDrawTileInfo[numCommandDrawTileInfo] = command;
			numCommandDrawTileInfo++;
		}
	}
	// низ
	if (cameraPosY < fpos.y)
	{
		if (z == 0 || !tiles->tiles[z - 1][x][y])
		{
			command.texture = tiles->tiles[z][x][y]->tileTemplate->textureBottom;
			command.side = TileSide::Bottom;

			if (numCommandDrawTileInfo == gCommandDrawTileInfo.size())
				gCommandDrawTileInfo.push_back(command);
			else
				gCommandDrawTileInfo[numCommandDrawTileInfo] = command;
			numCommandDrawTileInfo++;
		}
	}

#else
	const glm::mat4 translateMatrix = glm::translate(glm::mat4(1.0f), { fpos.x, fpos.y, fpos.z });

	// задн€€ сторона
	if (y == 0 || !tiles->tiles[z][x][y - 1])
	{
		drawSide(tiles->tiles[z][x][y]->textureBack, vp, translateMatrix, TileSide::Back);
	}
	// передн€€ сторона
	if (y == SizeMap - 1 || !tiles->tiles[z][x][y + 1])
	{
		drawSide(tiles->tiles[z][x][y]->textureForward, vp, translateMatrix, TileSide::Forward);
	}
	// лева€ сторона
	if (x == 0 || !tiles->tiles[z][x - 1][y])
	{
		drawSide(tiles->tiles[z][x][y]->textureLeft, vp, translateMatrix, TileSide::Left);
	}
	// права€
	if (x == SizeMap - 1 || !tiles->tiles[z][x + 1][y])
	{
		drawSide(tiles->tiles[z][x][y]->textureRight, vp, translateMatrix, TileSide::Right);
	}
	// вверх
	if (cameraPosY > fpos.y)
	{
		if (z == SizeMapZ - 1 || !tiles->tiles[z + 1][x][y])
			drawSide(tiles->tiles[z][x][y]->textureTop, vp, translateMatrix, TileSide::Top);
	}
	// низ
	if (cameraPosY < fpos.y)
	{
		if (z == 0 || !tiles->tiles[z - 1][x][y])
			drawSide(tiles->tiles[z][x][y]->textureBottom, vp, translateMatrix, TileSide::Bottom);
	}
#endif
}
//-----------------------------------------------------------------------------
void TileMapGeometry::drawSide(Texture2D* texture, const glm::mat4& VP, glm::mat4 worldMat, TileSide side)
{
	if (!texture) return;

	texture->Bind();

	constexpr float radM90 = glm::radians(-90.0f);
	constexpr float rad90 = glm::radians(90.0f);
	constexpr float rad0 = glm::radians(0.0f);
	constexpr float rad180 = glm::radians(180.0f);

	constexpr glm::vec3 axisX = { 1.0f, 0.0f, 0.0f };
	constexpr glm::vec3 axisY = { 0.0f, 1.0f, 0.0f };

	switch (side)
	{
	case TileSide::Top:
		worldMat = glm::rotate(worldMat, radM90, axisX);
		break;
	case TileSide::Bottom:
		worldMat = glm::rotate(worldMat, rad90, axisX);
		break;
	case TileSide::Forward:
		worldMat = glm::rotate(worldMat, rad0, axisY);
		break;
	case TileSide::Back:
		worldMat = glm::rotate(worldMat, rad180, axisY);
		break;
	case TileSide::Left:
		worldMat = glm::rotate(worldMat, radM90, axisY);
		break;
	case TileSide::Right:
		worldMat = glm::rotate(worldMat, rad90, axisY);
		break;
	}	

	m_shaderProgram.SetUniform(m_MatrixID, VP * worldMat);
	m_vao.Draw();
}
//-----------------------------------------------------------------------------