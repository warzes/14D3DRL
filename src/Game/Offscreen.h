#pragma once

#include "RenderResource.h"

class Offscreen
{
public:
	bool Init();
	void Close();

	void Bind();
	void DrawToScreen();

private:
	FrameBuffer m_fb;
	VertexArrayBuffer m_vaoQuad;
	VertexBuffer m_vertexBufQuad;
	ShaderProgram m_shaderProgramQuad;
};