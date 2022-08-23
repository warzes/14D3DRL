#pragma once

#include "RenderResource.h"
#include "Tile.h"
#include "CoreMath.h"

class Camera;
class TilesCell;

class TileMapGeometry
{
public:
	bool Init();
	void Close();

	void Draw(const Camera& camera, TilesCell* tiles);

private:
	void drawSide(Texture2D* texture, const glm::mat4& VP, const Vector3& pos, TileSide side);

	VertexArrayBuffer m_vao;
	VertexBuffer m_vertexBuf;
	IndexBuffer m_indexBuf;
	ShaderProgram m_shaderProgram;
	UniformVariable m_MatrixID;
	UniformVariable m_TextureID;
};