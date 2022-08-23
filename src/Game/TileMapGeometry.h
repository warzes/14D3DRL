#pragma once

#include "RenderResource.h"

class Camera;

class TileMapGeometry
{
public:
	bool Init();
	void Close();

	void Draw(const Camera& camera);

private:
	VertexArrayBuffer m_vao;
	VertexBuffer m_vertexBuf;
	IndexBuffer m_indexBuf;
	Texture2D* tex;
	ShaderProgram shaderProgram2;
	UniformVariable MatrixID;
	UniformVariable TextureID;
};
