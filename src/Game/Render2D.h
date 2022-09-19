#pragma once

#include "RenderResource.h"
#include "MathCore.h"

class Font;

class Text
{
public:
	bool Create(const std::string& fontFileName, uint32_t fontSize);
	void Destroy();

	void SetText(const std::wstring& text);
	void Draw(const Vector3& position, const Vector3& color, const glm::mat4& orthoMat);

private:
	bool create(Font* font);

	std::wstring m_text;
	Font* m_font = nullptr;
	GLuint vao = 0;
	GLuint vertexBuffer = 0;
	GLuint indexBuffer = 0;
	uint16_t indexElementCount = 0;
	float angle = 0;
};