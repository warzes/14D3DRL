#pragma once

#include "RenderResource.h"
#include "Tile.h"
#include "CoreMath.h"

class Camera;
class TilesCell;
class SimpleFrustum;

class TileMapGeometry
{
public:
	bool Init();
	void Close();

	void Draw(const Camera& camera, TilesCell* tiles);

private:
	void drawTile(const SimpleFrustum& frustum, int x, int y, int z, TilesCell* tiles, int cameraPosY, const Camera& camera);
	void drawSide(Texture2D* texture, glm::mat4 worldMat, TileSide side);

	VertexArrayBuffer m_vao;
	VertexBuffer m_vertexBuf;
	IndexBuffer m_indexBuf;
	ShaderProgram m_shaderProgram;
	//UniformVariable m_MatrixID;
	UniformVariable m_modelMatrixID;
	UniformVariable m_viewMatrixID;
	UniformVariable m_projectionMatrixID;
	UniformVariable m_color;

	UniformVariable m_TextureID;
};