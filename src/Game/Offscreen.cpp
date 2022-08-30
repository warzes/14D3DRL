#include "stdafx.h"
#include "Offscreen.h"
#include "Vertex.h"
#include "RenderResource.h"
//-----------------------------------------------------------------------------
// Game Boy Palette Mapping
static constexpr const char* GameBoyPaletteShader = R"(
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
static constexpr const char* Palette16FragmentShader = R"(
#version 330 core

in vec2 UV;

out vec4 fragColor;

uniform sampler2D screenTexture;

const int numColor = 16;

void main()
{
	vec3 color = texture(screenTexture, UV).rgb;
	const float gamma = 1.5;
	color.r = pow(color.r, gamma);
	color.g = pow(color.g, gamma);
	color.b = pow(color.b, gamma);

	vec3 col[numColor];

	col[0] = vec3(0.0, 0.0, 0.0);
	col[1] = vec3(0.616, 0.616, 0.616);
	col[2] = vec3(1.0, 1.0, 1.0);
	col[3] = vec3(0.745, 0.149, 0.2);
	col[4] = vec3(0.878, 0.435, 0.545);
	col[5] = vec3(0.286, 0.235, 0.169);
	col[6] = vec3(0.643, 0.392, 0.133);
	col[7] = vec3(0.922, 0.537, 0.192);
	col[8] = vec3(0.969, 0.886, 0.420);
	col[9] = vec3(0.184, 0.282, 0.306);
	col[10] = vec3(0.267, 0.537, 0.102);
	col[11] = vec3(0.639, 0.808, 0.153);
	col[12] = vec3(0.106, 0.149, 0.196);
	col[13] = vec3(0.0, 0.341, 0.518);
	col[14] = vec3(0.192, 0.635, 0.949);
	col[15] = vec3(0.698, 0.863, 0.937);

	//vec3 col0 = vec3(0.612, 0.725, 0.086);
	//vec3 col1 = vec3(0.549, 0.667, 0.078);
	//vec3 col2 = vec3(0.188, 0.392, 0.188);
	//col[3] = vec3(0.063, 0.247, 0.063);



	float dist[numColor];
	for(int i = 0; i < numColor; ++i)
	{
		dist[i] = length(color - col[i]);
	}

	float d = min(dist[0], dist[1]);
	for(int i = 2; i < numColor; ++i)
	{
		d = min(d, dist[i]);
	}

	color = vec3(1.0, 1.0, 1.0);
	for(int i =0; i < numColor; ++i)
	{
		if (d == dist[i])
		{
			color = col[i];
			break;
		}
	}

	fragColor = vec4(color, 1.0);
}
)";
//-----------------------------------------------------------------------------
static constexpr const char* OffscreenVertexShader = R"(
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
static constexpr const char* OffscreenFragmentShader = R"(
#version 330 core

in vec2 UV;

out vec4 fragColor;

uniform sampler2D screenTexture;

void main()
{
	//float PixelsX = 240.0;
	//float PixelsY = 120.0;
	//vec2 pos;
	//pos.x = floor(UV.x * PixelsX) / PixelsX;
	//pos.y = floor(UV.y * PixelsY) / PixelsY;
	//fragColor = texture( screenTexture, pos );
	
	fragColor = texture(screenTexture, UV);

	//vec3 c = texture(screenTexture, UV).rgb;
	//vec3 c1 = mix(vec3(0.0), c.rgb, 0.0625);
	//c1.rgb = vec3(float(int(c1.r * 230.0)) / 230.0, float(int(c1.g * 230.0)) / 230.0, float(int(c1.b * 230.0)) / 230.0);
	//c.rgb = mix(vec3(0.0), c1.rgb, 16.0);
	//fragColor = vec4(c, 1.0);

}
)";
//-----------------------------------------------------------------------------
extern int WindowWidth;
extern int WindowHeight;
//-----------------------------------------------------------------------------
bool Offscreen::Init()
{
	//m_shaderProgramQuad.CreateFromMemories(OffscreenVertexShader, Palette16FragmentShader);
	m_shaderProgramQuad.CreateFromMemories(OffscreenVertexShader, OffscreenFragmentShader);

	m_shaderProgramQuad.Bind();
	m_shaderProgramQuad.SetUniform("screenTexture", 0);

#if TileMapSimpleTexture
	m_fb.Create(320, 240);
#else
	m_fb.Create(320, 240);
#endif

	// Quad vertices
	static constexpr Vertex_Pos2_TexCoord quadVertices[] = {
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
	//m_fb.Bind({ 0.4, 0.5, 0.4 });
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