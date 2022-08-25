#include "stdafx.h"
#include "TileMapGeometry.h"
#include "Vertex.h"
#include "Camera.h"
#include "DrawPrimitive.h"
#include "Utility.h"
#include "TileMap.h"
#define DRAW_COMMAND 0
//-----------------------------------------------------------------------------
// TODO: проверить нормали. а направление света идет от игрока
constexpr const char* vertexShaderSource = R"(
#version 330 core

layout(location = 0) in vec3 aVertexPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTextCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 UV;
out vec3 normal;
out float visibility;

//const float density = 0.03;
const float density = 0.09;
const float gradient = 1.5;

void main()
{
	vec4 vertPosRelativeToCamera = view * model * vec4(aVertexPosition, 1.0);
	float dist = length(vertPosRelativeToCamera.xyz);
	visibility = exp(-pow((dist * density), gradient));
	visibility = clamp(visibility, 0.0, 1.0);

	gl_Position =  projection * vertPosRelativeToCamera;
	normal = mat3(transpose(inverse(model))) * aNormal;
	UV = aTextCoord;
}
)";
constexpr const char* fragmentShaderSource = R"(
#version 330 core

in vec2 UV;
in vec3 normal;
in float visibility;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D Texture0;

void main()
{
	vec3 fogColor = vec3(0.4, 0.5, 0.4);
	vec3 lightDirection = vec3(0.0, 0.8, 0.2);
	vec3 norm = normalize(normal);
	float shadow = dot(norm, lightDirection);
	if(shadow <= 0.0)
	{
		shadow = 0.0;
	}
	vec3 objectColor = texture(Texture0, UV).xyz * uColor;
	//vec3 objectColor = uColor;

	FragColor = (vec4(objectColor, 1.0) * (shadow + 0.3)) * 0.7;
	//FragColor = (vec4(objectColor, 1.0) * (shadow + 0.7)) * 0.7;
	FragColor = mix(vec4(fogColor, 1.0), FragColor, visibility);

	// HDR tonemapping
	//FragColor.rgb = FragColor.rgb / (FragColor.rgb + vec3(1.0));
	// gamma correct
	//FragColor.rgb = pow(FragColor.rgb, vec3(1.0 / 2.2));

	//FragColor = texture( Texture0, UV );
}
)";
//-----------------------------------------------------------------------------
constexpr const char* solidFragmentShaderSource = R"(
#version 330 core

in vec2 UV;
in vec3 normal;
in float visibility;

out vec4 FragColor;

uniform vec3 uColor;
uniform sampler2D Texture0;

void main()
{
	vec3 fogColor = vec3(0.4, 0.5, 0.4);
	vec3 lightDirection = vec3(0.0, 0.8, 0.2);
	vec3 norm = normalize(normal);
	float shadow = dot(norm, lightDirection);
	if(shadow <= 0.0)
	{
		shadow = 0.0;
	}
	vec3 objectColor = uColor;

	FragColor = (vec4(objectColor, 1.0) * (shadow + 0.3)) * 0.7;
	FragColor = mix(vec4(fogColor, 1.0), FragColor, visibility);


	vec3 color1 = vec3(0.1, 0.2, 0.3);
	vec3 color2 = vec3(0.8, 0.4, 0.2);

	vec4 color = FragColor;
	//color.rgb = color2;

	if (UV.x < 0.01 || UV.x > 0.99) color.rgb = color1;
	if (UV.y < 0.01 || UV.y > 0.99) color.rgb = color1;
	FragColor = color;
}
)";
//-----------------------------------------------------------------------------
bool TileMapGeometry::Init()
{
	m_shaderProgram.CreateFromMemories(vertexShaderSource, fragmentShaderSource);
	//m_shaderProgram.CreateFromMemories(vertexShaderSource, solidFragmentShaderSource);
	m_shaderProgram.Bind();
	//m_MatrixID = m_shaderProgram.GetUniformVariable("MVP");
	m_modelMatrixID = m_shaderProgram.GetUniformVariable("model");
	m_viewMatrixID = m_shaderProgram.GetUniformVariable("view");
	m_projectionMatrixID = m_shaderProgram.GetUniformVariable("projection");
	m_color = m_shaderProgram.GetUniformVariable("uColor");
	m_TextureID = m_shaderProgram.GetUniformVariable("Texture0");
	m_shaderProgram.SetUniform(m_TextureID, 0);
	m_shaderProgram.SetUniform(m_color, glm::vec3(1.0,1.0,1.0));

	constexpr Vertex_Pos3_Normal_TexCoord vertices[] =
	{
		/* pos                  normal              uvs */
		{ {-0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f} },
		{ { 0.5f, -0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f} },
		{ { 0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f} },
		{ {-0.5f,  0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}, {0.0f, 1.0f} },
	};
	constexpr uint16_t indices[] = { 0, 1, 2, 2, 3, 0 };

	m_vertexBuf.Create(RenderResourceUsage::Static, Countof(vertices), sizeof(Vertex_Pos3_Normal_TexCoord), vertices);
	m_indexBuf.Create(RenderResourceUsage::Static, Countof(indices), sizeof(uint16_t), indices);

	std::vector<VertexAttribute> attribs =
	{
		{.size = 3, .type = GL_FLOAT, .normalized = false, .stride = sizeof(Vertex_Pos3_Normal_TexCoord), .pointer = (void*)offsetof(Vertex_Pos3_Normal_TexCoord, position)},
		{.size = 3, .type = GL_FLOAT, .normalized = false, .stride = sizeof(Vertex_Pos3_Normal_TexCoord), .pointer = (void*)offsetof(Vertex_Pos3_Normal_TexCoord, normal)},
		{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(Vertex_Pos3_Normal_TexCoord), .pointer = (void*)offsetof(Vertex_Pos3_Normal_TexCoord, texCoord)},
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
	m_shaderProgram.Bind();
	m_shaderProgram.SetUniform(m_viewMatrixID, camera.GetViewMatrix());
	m_shaderProgram.SetUniform(m_projectionMatrixID, camera.GetProjectionMatrix());

	const int cameraPosX = floor(camera.GetPosition().x);
	const int cameraPosY = floor(camera.GetPosition().y);
	const int cameraPosZ = floor(camera.GetPosition().z);

	//const glm::mat4 vp = camera.GetProjectionMatrix() * camera.GetViewMatrix();

	SimpleFrustum frustum;
	frustum.Extract(camera.GetProjectionMatrix(), camera.GetViewMatrix());

#if DRAW_COMMAND
	numCommandDrawTileInfo = 0;
#endif

#if 0 // получилось медленее
	// новый способ рисования

	constexpr int viewDist = 40;

	for (int z = 0; z < SizeMapZ; z++)
	{
		drawTile(frustum, cameraPosX, cameraPosZ, z, tiles, cameraPosY, vp);

		// 1 ряд
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
#ifdef _DEBUG
		if (z == 0 && (int)cameraPosY < z) continue;
		if (z == SizeMapZ-1 && (int)cameraPosY >= z) continue;
#endif

		for (int x = cameraPosX - viewDist; x < cameraPosX + viewDist; x++)
		{
			if (x < 0 || x >= SizeMap) continue;
			for (int y = cameraPosZ - viewDist; y < cameraPosZ + viewDist; y++)
			{
				if (y < 0 || y >= SizeMap) continue;
				drawTile(frustum, x, y, z, tiles, cameraPosY, camera);
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
}
//-----------------------------------------------------------------------------
void TileMapGeometry::drawTile(const SimpleFrustum& frustum, int x, int y, int z, TilesCell* tiles, int cameraPosY, const Camera& camera)
{
	if (x < 0 || x >= SizeMap || y < 0 || y >= SizeMap || z < 0 || z >= SizeMapZ) return;
	if (tiles->tiles[z][x][y].tileInfoId == TileUnknown) return;

	const Vector3 fpos = { (float)x, z - 1.0f, (float)y };
	const Vector3 min = { fpos.x - 0.5f, fpos.y - 0.5f, fpos.z - 0.5f };
	const Vector3 max = { fpos.x + 0.5f, fpos.y + 0.5f, fpos.z + 0.5f };
	if (!frustum.AABBoxIn(min, max)) return;

#if DRAW_COMMAND
	CommandDrawTileInfo command;
	command.pos = { fpos.x, fpos.y, fpos.z };
	command.distance = glm::distance(camera.GetPosition(), command.pos);

	// задняя сторона
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
	// передняя сторона
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
	// левая сторона
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
	// правая
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

	auto& tileInfo = gTileTemplateManager.GetTileInfo(tiles->tiles[z][x][y]);

	// задняя сторона
	if (y == 0 || tiles->tiles[z][x][y - 1].tileInfoId == TileUnknown)
	{
		drawSide(tileInfo.textureBack, translateMatrix, TileSide::Back);
	}
	// передняя сторона
	if (y == SizeMap - 1 || tiles->tiles[z][x][y + 1].tileInfoId == TileUnknown)
	{
		drawSide(tileInfo.textureForward, translateMatrix, TileSide::Forward);
	}
	// левая сторона
	if (x == 0 || tiles->tiles[z][x - 1][y].tileInfoId == TileUnknown)
	{
		drawSide(tileInfo.textureLeft, translateMatrix, TileSide::Left);
	}
	// правая
	if (x == SizeMap - 1 || tiles->tiles[z][x + 1][y].tileInfoId == TileUnknown)
	{
		drawSide(tileInfo.textureRight, translateMatrix, TileSide::Right);
	}
	// вверх
	if (cameraPosY > fpos.y)
	{
		if (z == SizeMapZ - 1 || tiles->tiles[z + 1][x][y].tileInfoId == TileUnknown)
			drawSide(tileInfo.textureTop, translateMatrix, TileSide::Top);
	}
	// низ
	if (cameraPosY < fpos.y)
	{
		if (z == 0 || tiles->tiles[z - 1][x][y].tileInfoId == TileUnknown)
			drawSide(tileInfo.textureBottom, translateMatrix, TileSide::Bottom);
	}
#endif
}
//-----------------------------------------------------------------------------
void TileMapGeometry::drawSide(Texture2D* texture, glm::mat4 worldMat, TileSide side)
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

	m_shaderProgram.SetUniform(m_modelMatrixID, worldMat);
	m_vao.Draw();
}
//-----------------------------------------------------------------------------