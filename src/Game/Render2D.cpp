#include "stdafx.h"
#include "Render2D.h"
#include "Log.h"
#define STB_TRUETYPE_IMPLEMENTATION
#include <stb/stb_truetype.h>
//-----------------------------------------------------------------------------
class Font
{
public:
	uint32_t size = 60;
	std::string fontFileName = "../fonts/OpenSans-Regular.ttf";
	const uint32_t atlasWidth = 1024;
	const uint32_t atlasHeight = 1024;
	//const uint32_t oversampleX = 2;
	//const uint32_t oversampleY =2;
	const uint32_t firstCharENG = ' ';
	const uint32_t charCountENG = '~' - ' ' + 1;
	const uint32_t firstCharRUS = 0x400;
	const uint32_t charCountRUS = 0x452 - 0x400;

	std::unique_ptr<stbtt_packedchar[]> charInfo;
	Texture2D texture;
};
//-----------------------------------------------------------------------------
static std::vector<Font> m_cacheFont;
static ShaderProgram cacheShader;
static UniformVariable m_idAttributeTextColor;
static UniformVariable m_idAttributeWorldViewProjMatrix;
//-----------------------------------------------------------------------------
constexpr const char* fontVertexShaderSource = R"(
#version 330 core

layout (location = 0) in vec4 vertex; // <vec2 pos, vec2 tex>

uniform mat4 worldViewProjMatrix;

out vec2 uv0;

void main()
{
    gl_Position = worldViewProjMatrix * vec4(vertex.xy, 0.0, 1.0);
    uv0 = vertex.zw;
}
)";
constexpr const char* fontFragmentShaderSource = R"(
#version 330 core

in vec2 uv0;

uniform sampler2D mainTex;
uniform vec3 textColor;

out vec4 fragColor;

void main()
{
    vec4 sampled = vec4(1.0, 1.0, 1.0, texture(mainTex, uv0).r);
    fragColor = vec4(textColor, 1.0) * sampled;
}
)";
//-----------------------------------------------------------------------------
struct GlyphInfo
{
	Vector4 positions[4];
	float offsetX = 0;
	float offsetY = 0;
};
//-----------------------------------------------------------------------------
inline GlyphInfo makeGlyphInfo(Font* font, uint32_t character, float offsetX, float offsetY)
{
	stbtt_aligned_quad quad;

	int char_index = 0;
	if (character < font->firstCharENG + font->charCountENG)
		char_index = character - font->firstCharENG;
	else
		char_index = character - font->firstCharRUS + font->charCountENG;

	stbtt_GetPackedQuad(font->charInfo.get(), font->atlasWidth, font->atlasHeight, char_index, &offsetX, &offsetY, &quad, 1);

	const int sizeY = font->size / 2;

	GlyphInfo info{};
	info.offsetX = offsetX;
	info.offsetY = offsetY;
	info.positions[0] = { quad.x0, quad.y0 + sizeY, quad.s0, quad.t0 };
	info.positions[1] = { quad.x0, quad.y1 + sizeY, quad.s0, quad.t1 };
	info.positions[2] = { quad.x1, quad.y1 + sizeY, quad.s1, quad.t1 };
	info.positions[3] = { quad.x1, quad.y0 + sizeY, quad.s1, quad.t0 };

	return info;
}
//-----------------------------------------------------------------------------
Font* getFont(const std::string& fontFileName, uint32_t fontSize)
{
	Font* font = nullptr;
	for (int i = 0; i < m_cacheFont.size(); i++)
	{
		if (m_cacheFont[i].fontFileName == fontFileName && m_cacheFont[i].size == fontSize)
		{
			font = &m_cacheFont[i];
			break;
		}
	}
	if (!font)
	{
		Font font_;
		font_.size = fontSize;
		font_.fontFileName = fontFileName;

		std::ifstream file(fontFileName, std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			LogError("Failed to open file " + fontFileName);
			return nullptr;
		}

		const auto size = file.tellg();
		file.seekg(0, std::ios::beg);
		auto bytes = std::vector<uint8_t>(size);
		file.read(reinterpret_cast<char*>(&bytes[0]), size);
		file.close();

		auto atlasData = new uint8_t[font_.atlasWidth * font_.atlasHeight];

		font_.charInfo = std::make_unique<stbtt_packedchar[]>(font_.charCountENG + font_.charCountRUS);

		stbtt_pack_context context;
		//if (!stbtt_PackBegin(&context, atlasData.get(), font_.atlasWidth, font_.atlasHeight, 0, 1, nullptr))
		//	panic("Failed to initialize font");
		stbtt_PackBegin(&context, atlasData, font_.atlasWidth, font_.atlasHeight, 0, 1, nullptr);

		//stbtt_PackSetOversampling(&context, font_.oversampleX, font_.oversampleY);
		//if (!stbtt_PackFontRange(&context, fontData.data(), 0, font_.size, font_.firstChar, font_.charCount, font_.charInfo.get()))
		//    panic("Failed to pack font");

		//stbtt_PackFontRange(&context, fontData.data(), 0, font_.size, font_.firstChar, font_.charCount, font_.charInfo.get());
		stbtt_PackFontRange(&context, bytes.data(), 0, font_.size, font_.firstCharENG, font_.charCountENG, font_.charInfo.get());
		stbtt_PackFontRange(&context, bytes.data(), 0, font_.size, font_.firstCharRUS, font_.charCountRUS, font_.charInfo.get() + font_.charCountENG);

		stbtt_PackEnd(&context);

		Texture2DCreateInfo createInfo;
		createInfo.format = TexelsFormat::R_U8;
		//createInfo.minFilter = TextureMinFilter::Linear;
		//createInfo.magFilter = TextureMagFilter::Linear;
		createInfo.width = font_.atlasWidth;
		createInfo.height = font_.atlasHeight;
		createInfo.depth = 1;
		createInfo.data = atlasData;
		createInfo.mipmap = false;

		//font_.texture.CreateFromMemories(GL_RGB, GL_RED, GL_UNSIGNED_BYTE, font_.atlasWidth, font_.atlasHeight, atlasData.get());
		font_.texture.CreateFromMemories(createInfo);

		delete[] atlasData;

		m_cacheFont.push_back(std::move(font_));
		font = &m_cacheFont[m_cacheFont.size() - 1];
	}

	return font;
}
//-----------------------------------------------------------------------------
bool Text::Create(const std::string& fontFileName, uint32_t fontSize)
{
	Font* font = getFont(fontFileName, fontSize);
	if (!font || !create(font))
	{
		LogError("Text not create!");
		return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
void Text::Destroy()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteBuffers(1, &indexBuffer);
}
//-----------------------------------------------------------------------------
void Text::SetText(const std::wstring& text)
{
	if (m_font && m_text != text)
	{
		m_text = text;
		std::vector<Vector4> vertices;
		std::vector<uint16_t> indexes;

		uint16_t lastIndex = 0;
		float offsetX = 0, offsetY = 0;
		for (auto c : text)
		{
			const auto glyphInfo = makeGlyphInfo(m_font, c, offsetX, offsetY);
			offsetX = glyphInfo.offsetX;
			offsetY = glyphInfo.offsetY;

			vertices.emplace_back(glyphInfo.positions[0]);
			vertices.emplace_back(glyphInfo.positions[1]);
			vertices.emplace_back(glyphInfo.positions[2]);
			vertices.emplace_back(glyphInfo.positions[3]);
			indexes.push_back(lastIndex);
			indexes.push_back(lastIndex + 1);
			indexes.push_back(lastIndex + 2);
			indexes.push_back(lastIndex);
			indexes.push_back(lastIndex + 2);
			indexes.push_back(lastIndex + 3);

			lastIndex += 4;
		}
		indexElementCount = indexes.size();

		glBindVertexArray(vao);
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(Vector4) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint16_t) * indexElementCount, indexes.data(), GL_STATIC_DRAW);
	}
}
//-----------------------------------------------------------------------------
void Text::Draw(const Vector3& position, const Vector3& color, const glm::mat4& orthoMat)
{
	if (m_text.empty() || !m_font || !m_font->texture.IsValid())
		return;

	const glm::mat4 pm = orthoMat * glm::translate(glm::mat4(1.0f), glm::vec3(position.x, position.y, position.z));

	cacheShader.Bind();
	cacheShader.SetUniform(m_idAttributeTextColor, { color.x, color.y, color.z });
	cacheShader.SetUniform(m_idAttributeWorldViewProjMatrix, pm);

	m_font->texture.Bind(0);
	glBindVertexArray(vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
	glDrawElements(GL_TRIANGLES, indexElementCount, GL_UNSIGNED_SHORT, nullptr);
	VertexArrayBuffer::UnBind();
}
//-----------------------------------------------------------------------------
bool Text::create(Font* font)
{
	m_font = font;

	if (!cacheShader.IsValid())
	{
		cacheShader.CreateFromMemories(fontVertexShaderSource, fontFragmentShaderSource);
		cacheShader.Bind();
		m_idAttributeTextColor = cacheShader.GetUniformVariable("textColor");
		m_idAttributeWorldViewProjMatrix = cacheShader.GetUniformVariable("worldViewProjMatrix");
		cacheShader.SetUniform("mainTex", 0);
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vertexBuffer);
	glGenBuffers(1, &indexBuffer);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	return true;
}
//-----------------------------------------------------------------------------