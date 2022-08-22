#include "stdafx.h"
#include "RenderResource.h"
#include "Camera.h"
#include "DrawPrimitive.h"

Camera camera;

VertexArrayBuffer vao;
VertexBuffer vertexBuf;
IndexBuffer indexBuf;

extern int WindowWidth;
extern int WindowHeight;

struct vertex_t
{
	glm::vec3 position;
	glm::vec2 texCoord;
};

struct VertexPos2Tex2
{
	glm::vec2 position;
	glm::vec2 texCoord;
};

Texture2D texture;

// Embedded vertex shader source.
const char* g_sample_vs_src = R"(
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

// Embedded fragment shader source.
const char* g_sample_fs_src2 = R"(
#version 330 core

in vec2 UV;

out vec4 color;

uniform sampler2D Texture0;

void main()
{
	color = texture( Texture0, UV );
}
)";

// Game Boy Palette Mapping
// использовать этот шейдер для фреймкадра, а не отдельных объектов
const char* GameBoyPaletteShader = R"(
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

ShaderProgram shaderProgram2;
UniformVariable MatrixID;
UniformVariable TextureID;


const char* OffscreenVertexShader = R"(
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

const char* OffscreenFragmentShader = R"(
#version 330 core

in vec2 UV;

out vec4 fragColor;

uniform sampler2D screenTexture;

void main()
{
	//float PixelsX = 80.0;
	//float PixelsY = 60.0;
	//vec2 pos;
	//pos.x = floor(UV.x * PixelsX) / PixelsX;
	//pos.y = floor(UV.y * PixelsY) / PixelsY;
	//fragColor = texture( screenTexture, pos );
	fragColor = texture(screenTexture, UV);
}
)";

// Quad vertices

VertexPos2Tex2 quadVertices[] = {
	{{-1.0f,  1.0f},  {0.0f, 1.0f}},
	{{ 1.0f,  1.0f},  {1.0f, 1.0f}},
	{{ 1.0f, -1.0f},  {1.0f, 0.0f}},

	{{ 1.0f, -1.0f},  {1.0f, 0.0f}},
	{{-1.0f, -1.0f},  {0.0f, 0.0f}},
	{{-1.0f,  1.0f},  {0.0f, 1.0f}},
};

FrameBuffer fb;
VertexArrayBuffer vaoQuad;
VertexBuffer vertexBufQuad;
ShaderProgram shaderProgramQuad;

//-----------------------------------------------------------------------------
bool GameAppInit()
{
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	//shaderProgram2.CreateFromMemories(g_sample_vs_src, GameBoyPaletteShader);
	shaderProgram2.CreateFromMemories(g_sample_vs_src, g_sample_fs_src2);
	shaderProgram2.Bind();
	MatrixID = shaderProgram2.GetUniformVariable("MVP");
	TextureID = shaderProgram2.GetUniformVariable("Texture0");
	shaderProgram2.SetUniform(TextureID, 0);

	//shaderProgramQuad.CreateFromMemories(OffscreenVertexShader, GameBoyPaletteShader);
	shaderProgramQuad.CreateFromMemories(OffscreenVertexShader, OffscreenFragmentShader);
	shaderProgramQuad.Bind();
	shaderProgramQuad.SetUniform("screenTexture", 0);

	Texture2DLoaderInfo textureLoaderInfo;
	textureLoaderInfo.fileName = "../data/textures/1.png";
	//textureLoaderInfo.mipmap = false;
	texture.CreateFromFiles(textureLoaderInfo);

	vertex_t vertices[] = {
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

	vertexBuf.Create(RenderResourceUsage::Static, 24, sizeof(vertex_t), vertices);

	uint16_t indices[] = {
	   0, 1, 2,  0, 2, 3,
	   6, 5, 4,  7, 6, 4,
	   8, 9, 10,  8, 10, 11,
	   14, 13, 12,  15, 14, 12,
	   16, 17, 18,  16, 18, 19,
	   22, 21, 20,  23, 22, 20
	};

	indexBuf.Create(RenderResourceUsage::Static, 36, sizeof(uint16_t), indices);
	
	std::vector<VertexAttribute> attribs =
	{
		{.size = 3, .type = GL_FLOAT, .normalized = false, .stride = sizeof(vertex_t), .pointer = (void*)offsetof(vertex_t, position)},
		{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(vertex_t), .pointer = (void*)offsetof(vertex_t, texCoord)},
	};
	vao.Create({ &vertexBuf }, &indexBuf, attribs);

	fb.Create(640, 480);
	vertexBufQuad.Create(RenderResourceUsage::Static, 6, sizeof(VertexPos2Tex2), quadVertices);
	std::vector<VertexAttribute> attribs2 =
	{
		{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(VertexPos2Tex2), .pointer = (void*)offsetof(VertexPos2Tex2, position)},
		{.size = 2, .type = GL_FLOAT, .normalized = false, .stride = sizeof(VertexPos2Tex2), .pointer = (void*)offsetof(VertexPos2Tex2, texCoord)}
	};
	vaoQuad.Create({ &vertexBufQuad }, nullptr, attribs2);

	
	return true;
}
//-----------------------------------------------------------------------------
void GameAppUpdate()
{
	camera.SimpleMove(0.01f);
}
//-----------------------------------------------------------------------------
void GameAppFrame()
{
	glm::mat4 ProjectionMatrix = camera.GetProjectionMatrix();
	glm::mat4 ViewMatrix = camera.GetViewMatrix();
	glm::mat4 RotateMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));	
	glm::mat4 TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, -4));
	glm::mat4 ModelMatrix = TranslateMatrix * RotateMatrix;
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	fb.Bind();

	shaderProgram2.Bind();
	shaderProgram2.SetUniform(MatrixID, MVP);
	texture.Bind();
	vao.Draw();

	{
		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2, 0, -2));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		shaderProgram2.SetUniform(MatrixID, MVP);
		vao.Draw();

		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2, 0, -2));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		shaderProgram2.SetUniform(MatrixID, MVP);
		vao.Draw();



		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-4, 0, 0));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		shaderProgram2.SetUniform(MatrixID, MVP);
		vao.Draw();

		TranslateMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(4, 0, 0));
		MVP = ProjectionMatrix * ViewMatrix * TranslateMatrix;
		shaderProgram2.SetUniform(MatrixID, MVP);
		vao.Draw();
	}



	//drawPrimitive::DrawCubeWires(camera, glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(2.0f), glm::vec3(0.0f, glm::radians(45.0f),0.0f), glm::vec4(0.4f, 1.0f, 0.4f, 0.7f), true);

	// Main Screen
	FrameBuffer::MainFrameBufferBind();
	glViewport(0, 0, WindowWidth, WindowHeight);
	shaderProgramQuad.Bind();
	fb.BindTextureBuffer();
	vaoQuad.Draw();

	Texture2D::UnBind();
}
//-----------------------------------------------------------------------------
void GameAppClose()
{
	vertexBuf.Destroy();
	indexBuf.Destroy();
	shaderProgram2.Destroy();
	texture.Destroy();
	vao.Destroy();
}
//-----------------------------------------------------------------------------