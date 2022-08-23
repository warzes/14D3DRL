#pragma once

#include "RenderResource.h"
#include "Tile.h"
#include "CoreMath.h"

class Camera;

class TileMapGeometry
{
public:
	bool Init();
	void Close();

	void Draw(const Camera& camera);

private:
	void drawSide(const Camera& camera, const Vector3& pos, TileSide side);

	VertexArrayBuffer m_vao;
	VertexBuffer m_vertexBuf;
	IndexBuffer m_indexBuf;
	ShaderProgram m_shaderProgram;
	UniformVariable m_MatrixID;
	UniformVariable m_TextureID;

	Texture2D* tex;
};
