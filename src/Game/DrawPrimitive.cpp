#include "stdafx.h"
#include "DrawPrimitive.h"
#include "Camera.h"
#include "RenderResource.h"
//-----------------------------------------------------------------------------
void drawPrimitive::DrawLine(const Camera& camera, const glm::vec3& startPos, const glm::vec3& endPos)
{
	static bool isCreate = false;
	static VertexArrayBuffer vao;
	static VertexBuffer vertexBuf;
	static ShaderProgram shaderProgram;
	static UniformVariable MatrixID;

	if (!isCreate)
	{
		isCreate = true;

		const float vertexData[] =
		{
			startPos.x, startPos.y, startPos.z,// 0
			endPos.x, endPos.y,  endPos.z// 1
		};

		vertexBuf.Create(RenderResourceUsage::Dynamic, 2, 3 * sizeof(float), vertexData);

		const std::vector<VertexAttribute> attribs =
		{
			{.size = 3, .type = GL_FLOAT, .normalized = false, .stride = 0, .pointer = (void*)0},
		};
		vao.Create(&vertexBuf, nullptr, attribs);

		const char* vertexSource = R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition;
uniform mat4 MVP;
void main()
{
gl_Position =  MVP * vec4(vertexPosition, 1);
}
)";

		const char* fragmentSource = R"(
#version 330 core
out vec4 outColor;
void main()
{
outColor = vec4(1.0, 1.0, 1.0, 1.0);
}
)";
		shaderProgram.CreateFromMemories(vertexSource, fragmentSource);
		shaderProgram.Bind();
		MatrixID = shaderProgram.GetUniformVariable("MVP");
	}
	else
	{
		const float vertexData[] =
		{
			startPos.x, startPos.y, startPos.z,// 0
			endPos.x, endPos.y,  endPos.z// 1
		};
		vertexBuf.Update(0, sizeof(vertexData), vertexData);
	}

	//const glm::mat4 ModelMatrix = glm::translate(glm::mat4(1.0f), position);
	const glm::mat4 MVP = camera.GetProjectionMatrix() * camera.GetViewMatrix()/* * worldMModelMatrixatrix*/;

	shaderProgram.Bind();
	shaderProgram.SetUniform(MatrixID, MVP);
	glLineWidth(4);
	glDisable(GL_DEPTH_TEST);
	vao.Draw(PrimitiveDraw::Lines);
	glLineWidth(1);
	glEnable(GL_DEPTH_TEST);
}
//-----------------------------------------------------------------------------
void drawPrimitive::DrawCubeWires(const Camera& camera, const glm::mat4& worldMatrix, const glm::vec4& color, bool disableDepthTest)
{
	static bool isCreate = false;
	static VertexArrayBuffer vao;
	static VertexBuffer vertexBuf;
	static IndexBuffer indexBuf;
	static ShaderProgram shaderProgram;
	static UniformVariable MatrixID;
	static UniformVariable ColorID;

	if (!isCreate)
	{
		isCreate = true;

		constexpr float vertexData[] =
		{
			-0.5f, -0.5f,  0.5f,// 0
				0.5f, -0.5f,  0.5f,// 1
				0.5f,  0.5f,  0.5f,// 2
			-0.5f,  0.5f,  0.5f,// 3
			-0.5f, -0.5f, -0.5f,// 4
				0.5f, -0.5f, -0.5f,// 5
				0.5f,  0.5f, -0.5f,// 6
			-0.5f,  0.5f, -0.5f,// 7
		};
		constexpr uint16_t indexData[] =
		{
			0, 1,
			1, 2,
			2, 3,
			3, 0,
			4, 5,
			5, 6,
			6, 7,
			7, 4,
			3, 7,
			2, 6,
			0, 4,
			1, 5
		};

		vertexBuf.Create(RenderResourceUsage::Static, 8, 3 * sizeof(float), vertexData);
		indexBuf.Create(RenderResourceUsage::Static, 24, sizeof(uint16_t), indexData);

		const std::vector<VertexAttribute> attribs =
		{
			{.size = 3, .type = GL_FLOAT, .normalized = false, .stride = 0, .pointer = (void*)0},
		};
		vao.Create(&vertexBuf, &indexBuf, attribs);

		const char* vertexSource = R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition;
uniform mat4 MVP;
void main()
{
gl_Position =  MVP * vec4(vertexPosition, 1);
}
)";

		const char* fragmentSource = R"(
#version 330 core
uniform vec4 inColor;
out vec4 outColor;
void main()
{
outColor = inColor;
}
)";
		shaderProgram.CreateFromMemories(vertexSource, fragmentSource);
		shaderProgram.Bind();
		MatrixID = shaderProgram.GetUniformVariable("MVP");
		ColorID = shaderProgram.GetUniformVariable("inColor");
	}

	const glm::mat4 MVP = camera.GetProjectionMatrix() * camera.GetViewMatrix() * worldMatrix;

	shaderProgram.Bind();
	shaderProgram.SetUniform(MatrixID, MVP);
	shaderProgram.SetUniform(ColorID, color);
	if (disableDepthTest) glDisable(GL_DEPTH_TEST);
	//glLineWidth(4);
	vao.Draw(PrimitiveDraw::Lines);
	if (disableDepthTest) glEnable(GL_DEPTH_TEST);
}
//-----------------------------------------------------------------------------
void drawPrimitive::DrawCubeWires(const Camera& camera, const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotationRadian, const glm::vec4& color, bool disableDepthTest)
{
	const glm::mat4 transformX = glm::rotate(glm::mat4(1.0f), rotationRadian.x, glm::vec3(1.0f, 0.0f, 0.0f));
	const glm::mat4 transformY = glm::rotate(glm::mat4(1.0f), rotationRadian.y, glm::vec3(0.0f, 1.0f, 0.0f));
	const glm::mat4 transformZ = glm::rotate(glm::mat4(1.0f), rotationRadian.z, glm::vec3(0.0f, 0.0f, 1.0f));
	const glm::mat4 roationMatrix = transformY * transformX * transformZ;

	// translation * rotation * scale (also know as TRS matrix)
	const glm::mat4 ModelMatrix =
		glm::translate(glm::mat4(1.0f), position)
		* roationMatrix
		* glm::scale(glm::mat4(1.0f), size);

	drawPrimitive::DrawCubeWires(camera, ModelMatrix, color, disableDepthTest);
}
//-----------------------------------------------------------------------------