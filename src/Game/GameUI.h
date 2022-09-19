#pragma once

#include "RenderResource.h"

class TileMap;

class GameUI
{
public:
	bool Init();
	void Close();

	void Draw(const glm::vec3& newPlayerPos, const TileMap& map);

private:
	VertexArrayBuffer m_vaoQuad;
	VertexBuffer m_vertexBufQuad;
	ShaderProgram m_shaderProgramQuad;
	UniformVariable m_matrixID;
	UniformVariable m_colorID;

	glm::mat4 m_ortho;
	glm::mat4 m_orthoUI;

	int m_windowWidth = 0;
	int m_windowHeight = 0;

	float m_uiWidth = 0.0f;
	float m_uiHeight = 0.0f;
};