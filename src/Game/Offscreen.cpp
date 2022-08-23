#include "stdafx.h"
#include "Offscreen.h"
#include "Vertex.h"
#include "RenderResource.h"
//-----------------------------------------------------------------------------
// Game Boy Palette Mapping
constexpr const char* GameBoyPaletteShader = R"(
#version 330 core

in vec2 UV;

out vec4 fragColor;

uniform sampler2D screenTexture;

void main()
{
	vec3 color = texture(screenTexture, UV).rgb;

	float gamma = 1.5;
	color.r = pow(color.r, gamma);
	color.g = pow(color.g, gamma);
	color.b = pow(color.b, gamma);

	vec3 col1 = vec3(0.612, 0.725, 0.086);
	vec3 col2 = vec3(0.549, 0.667, 0.078);
	vec3 col3 = vec3(0.188, 0.392, 0.188);
	vec3 col4 = vec3(0.063, 0.247, 0.063);

	float dist1 = length(color - col1);
	float dist2 = length(color - col2);
	float dist3 = length(color - col3);
	float dist4 = length(color - col4);

	float d = min(dist1, dist2);
	d = min(d, dist3);
	d = min(d, dist4);

	if (d == dist1) {
	color = col1;
	}
	else if (d == dist2) {
	color = col2;
	}
	else if (d == dist3) {
	color = col3;
	}
	else {
	color = col4;
	} 

	fragColor = vec4(color, 1.0).rgba;
}
)";
//-----------------------------------------------------------------------------
constexpr const char* OffscreenVertexShader = R"(
#version 330 core

layout(location = 0) in vec2 vertexPosition;
layout (location = 1) in vec2 texCoords;

out vec2 UV;

void main()
{
	gl_Position = vec4(vertexPosition.x, vertexPosition.y, 0.0, 1.0); 
	UV = texCoords;
}
)";
//-----------------------------------------------------------------------------
constexpr const char* OffscreenFragmentShader = R"(
#version 330 core

in vec2 UV;

out vec4 fragColor;

uniform sampler2D screenTexture;

void main()
{
	//float PixelsX = 320.0;
	//float PixelsY = 240.0;
	//vec2 pos;
	//pos.x = floor(UV.x * PixelsX) / PixelsX;
	//pos.y = floor(UV.y * PixelsY) / PixelsY;
	//fragColor = texture( screenTexture, pos );
	fragColor = texture(screenTexture, UV);
}
)";
//-----------------------------------------------------------------------------
extern int WindowWidth;
extern int WindowHeight;
//-----------------------------------------------------------------------------
bool Offscreen::Init()
{
	//shaderProgramQuad.CreateFromMemories(OffscreenVertexShader, GameBoyPaletteShader);
	m_shaderProgramQuad.CreateFromMemories(OffscreenVertexShader, OffscreenFragmentShader);
	m_shaderProgramQuad.Bind();
	m_shaderProgramQuad.SetUniform("screenTexture", 0);

	m_fb.Create(640, 480);

	// Quad vertices
	constexpr Vertex_Pos2_TexCoord quadVertices[] = {
		{{-1.0f,  1.0f},  {0.0f, 1.0f}},
		{{ 1.0f,  1.0f},  {1.0f, 1.0f}},
		{{ 1.0f, -1.0f},  {1.0f, 0.0f}},

		{{ 1.0f, -1.0f},  {1.0f, 0.0f}},
		{{-1.0f, -1.0f},  {0.0f, 0.0f}},
		{{-1.0f,  1.0f},  {0.0f, 1.0f}},
	};

	m_vertexBufQuad.Create(RenderResourceUsage::Static, 6, sizeof(Vertex_Pos2_TexCoord), quadVertices);
	std::vector<VertexAttribute> attribs2 =
	{
		{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(Vertex_Pos2_TexCoord), .pointer = (void*)offsetof(Vertex_Pos2_TexCoord, position)},
		{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(Vertex_Pos2_TexCoord), .pointer = (void*)offsetof(Vertex_Pos2_TexCoord, texCoord)}
	};
	m_vaoQuad.Create({ &m_vertexBufQuad }, nullptr, attribs2);

	return true;
}
//-----------------------------------------------------------------------------
void Offscreen::Close()
{
	m_fb.Destroy();
	m_vaoQuad.Destroy();
	m_vertexBufQuad.Destroy();
	m_shaderProgramQuad.Destroy();
}
//-----------------------------------------------------------------------------
void Offscreen::Bind()
{
	m_fb.Bind({0.1, 0.3, 0.6});
}
//-----------------------------------------------------------------------------
void Offscreen::DrawToScreen()
{
	// Main Screen
	FrameBuffer::MainFrameBufferBind();
	glViewport(0, 0, WindowWidth, WindowHeight);
	m_shaderProgramQuad.Bind();
	m_fb.BindTextureBuffer();
	m_vaoQuad.Draw();
	Texture2D::UnBind();
}
//-----------------------------------------------------------------------------