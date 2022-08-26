#include "stdafx.h"
#include "RenderResource.h"
#include "Log.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>
//=============================================================================
// Cache Render State
//=============================================================================
namespace
{
	unsigned currentShaderProgram = 0;
	unsigned currentTexture2D[8] = { 0 };
	unsigned currentVAO = 0;
}
//=============================================================================
inline GLenum translate(RenderResourceUsage usage)
{
	switch (usage)
	{
	case RenderResourceUsage::Static:  return GL_STATIC_DRAW;
	case RenderResourceUsage::Dynamic: return GL_DYNAMIC_DRAW;
	case RenderResourceUsage::Stream:  return GL_STREAM_DRAW;
	}
	return 0;
}
//=============================================================================
bool ShaderProgram::CreateFromMemories(const std::string& vertexShaderMemory, const std::string& fragmentShaderMemory)
{
	if (vertexShaderMemory == "" || fragmentShaderMemory == "") return false;
	if (m_id > 0) Destroy();

	GLuint shaderVertex = createShader(GL_VERTEX_SHADER, vertexShaderMemory);
	GLuint shaderFragment = createShader(GL_FRAGMENT_SHADER, fragmentShaderMemory);

	m_id = glCreateProgram();
	glAttachShader(m_id, shaderVertex);
	glAttachShader(m_id, shaderFragment);

	glLinkProgram(m_id);
	GLint success = 0;
	glGetProgramiv(m_id, GL_LINK_STATUS, &success);
	if (success == GL_FALSE)
	{
		char log[512];
		glGetProgramInfoLog(m_id, 512, nullptr, log);
		LogError(("OPENGL: Shader program linking failed: " + std::string(log)).c_str());
		glDeleteProgram(m_id);
		m_id = 0;
	}

	glDeleteShader(shaderVertex);
	glDeleteShader(shaderFragment);

	return IsValid();
}
//-----------------------------------------------------------------------------
void ShaderProgram::Destroy()
{
	if (m_id > 0)
	{
		if (currentShaderProgram == m_id) UnBind();
		glDeleteProgram(m_id);
		m_id = 0;
	}
}
//-----------------------------------------------------------------------------
void ShaderProgram::Bind()
{
	if (currentShaderProgram != m_id)
	{
		currentShaderProgram = m_id;
		glUseProgram(currentShaderProgram);
	}
}
//-----------------------------------------------------------------------------
void ShaderProgram::UnBind()
{
	currentShaderProgram = 0;
	glUseProgram(0);
}
//-----------------------------------------------------------------------------
UniformVariable ShaderProgram::GetUniformVariable(const char* name)
{
	return { glGetUniformLocation(m_id, name) };
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(UniformVariable var, int value)
{
	glUniform1i(var.id, value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(UniformVariable var, float value)
{
	glUniform1f(var.id, value);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(UniformVariable var, const glm::vec2& v)
{
	glUniform2fv(var.id, 1, glm::value_ptr(v));
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(UniformVariable var, float x, float y, float z)
{
	glUniform3f(var.id, x, y, z);
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(UniformVariable var, const glm::vec3& v)
{
	glUniform3fv(var.id, 1, glm::value_ptr(v));
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(UniformVariable var, const glm::vec4& v)
{
	glUniform4fv(var.id, 1, glm::value_ptr(v));
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(UniformVariable var, const glm::mat3& mat)
{
	glUniformMatrix3fv(var.id, 1, GL_FALSE, glm::value_ptr(mat));
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(UniformVariable var, const glm::mat4& mat)
{
	glUniformMatrix4fv(var.id, 1, GL_FALSE, glm::value_ptr(mat));
}
//-----------------------------------------------------------------------------
void ShaderProgram::SetUniform(const std::string& name, int value)
{
	SetUniform(GetUniformVariable(name.c_str()), value);
}
void ShaderProgram::SetUniform(const std::string& name, float value)
{
	SetUniform(GetUniformVariable(name.c_str()), value);
}
void ShaderProgram::SetUniform(const std::string& name, float x, float y, float z)
{
	SetUniform(GetUniformVariable(name.c_str()), x, y, z);
}
void ShaderProgram::SetUniform(const std::string& name, const glm::vec2& v)
{
	SetUniform(GetUniformVariable(name.c_str()), v);
}
void ShaderProgram::SetUniform(const std::string& name, const glm::vec3& v)
{
	SetUniform(GetUniformVariable(name.c_str()), v);
}
void ShaderProgram::SetUniform(const std::string& name, const glm::vec4& v)
{
	SetUniform(GetUniformVariable(name.c_str()), v);
}
void ShaderProgram::SetUniform(const std::string& name, const glm::mat3& mat)
{
	SetUniform(GetUniformVariable(name.c_str()), mat);
}
void ShaderProgram::SetUniform(const std::string& name, const glm::mat4& mat)
{
	SetUniform(GetUniformVariable(name.c_str()), mat);
}
//-----------------------------------------------------------------------------
GLuint ShaderProgram::createShader(GLenum type, const std::string& source) const
{
	if (source == "") return 0;

	const char* shaderString = source.c_str();
	const GLuint shaderId = glCreateShader(type);
	glShaderSource(shaderId, 1, &shaderString, nullptr);
	glCompileShader(shaderId);

	GLint success = 0;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderId, 512, nullptr, infoLog);
		glDeleteShader(shaderId);
		const std::string msg = "Shader compilation failed : " + std::string(infoLog) + ", Source: " + source;
		LogError(msg.c_str());
		return 0;
	}

	return shaderId;
}
//=============================================================================
inline GLint translate(TextureWrapping wrap)
{
	switch (wrap)
	{
	case TextureWrapping::Repeat:         return GL_REPEAT;
	case TextureWrapping::MirroredRepeat: return GL_MIRRORED_REPEAT;
	case TextureWrapping::Clamp:          return GL_CLAMP_TO_EDGE;
	}
	return 0;
}
//-----------------------------------------------------------------------------
inline GLint translate(TextureMinFilter filter)
{
	switch (filter)
	{
	case TextureMinFilter::Nearest:              return GL_NEAREST;
	case TextureMinFilter::Linear:               return GL_LINEAR;
	case TextureMinFilter::NearestMipmapNearest: return GL_NEAREST_MIPMAP_NEAREST;
	case TextureMinFilter::NearestMipmapLinear:  return GL_NEAREST_MIPMAP_LINEAR;
	case TextureMinFilter::LinearMipmapNearest:  return GL_LINEAR_MIPMAP_NEAREST;
	case TextureMinFilter::LinearMipmapLinear:   return GL_LINEAR_MIPMAP_LINEAR;
	}
	return 0;
}
//-----------------------------------------------------------------------------
inline GLint translate(TextureMagFilter filter)
{
	switch (filter)
	{
	case TextureMagFilter::Nearest: return GL_NEAREST;
	case TextureMagFilter::Linear:  return GL_LINEAR;
	}
	return 0;
}
//-----------------------------------------------------------------------------
inline bool getTextureFormatType(TexelsFormat inFormat, GLenum textureType, GLenum& format, GLint& internalFormat, GLenum& oglType)
{
	if (inFormat == TexelsFormat::R_U8)
	{
		format = GL_RED;
		internalFormat = GL_R8;
		oglType = GL_UNSIGNED_BYTE;
	}
	else if (inFormat == TexelsFormat::RG_U8)
	{
		format = GL_RG;
		internalFormat = GL_RG8;
		oglType = GL_UNSIGNED_BYTE;
		const GLint swizzleMask[] = { GL_RED, GL_RED, GL_RED, GL_GREEN };
		glTexParameteriv(textureType, GL_TEXTURE_SWIZZLE_RGBA, swizzleMask); // TODO: могут быть проблемы с браузерами, тогда только грузить stb с указанием нужного формата
	}
	else if (inFormat == TexelsFormat::RGB_U8)
	{
		format = GL_RGB;
		internalFormat = GL_RGB8;
		oglType = GL_UNSIGNED_BYTE;
	}
	else if (inFormat == TexelsFormat::RGBA_U8)
	{
		format = GL_RGBA;
		internalFormat = GL_RGBA8;
		oglType = GL_UNSIGNED_BYTE;
	}
	else if (inFormat == TexelsFormat::Depth_U16)
	{
		format = GL_DEPTH_COMPONENT;
		internalFormat = GL_DEPTH_COMPONENT16;
		oglType = GL_UNSIGNED_SHORT;
	}
	else if (inFormat == TexelsFormat::DepthStencil_U16)
	{
		format = GL_DEPTH_STENCIL;
		internalFormat = GL_DEPTH24_STENCIL8;
		oglType = GL_UNSIGNED_SHORT;
	}
	else if (inFormat == TexelsFormat::Depth_U24)
	{
		format = GL_DEPTH_COMPONENT;
		internalFormat = GL_DEPTH_COMPONENT24;
		oglType = GL_UNSIGNED_INT;
	}
	else if (inFormat == TexelsFormat::DepthStencil_U24)
	{
		format = GL_DEPTH_STENCIL;
		internalFormat = GL_DEPTH24_STENCIL8;
		oglType = GL_UNSIGNED_INT;
	}	
	else
	{
		LogError("unknown texture format");
		return false;
	}
	return true;
}
//-----------------------------------------------------------------------------
bool Texture2D::CreateFromMemories(const Texture2DCreateInfo& createInfo)
{
	if (id > 0) Destroy();

	// save prev pixel store state
	GLint Alignment = 0;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &Alignment);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// gen texture res
	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, translate(createInfo.wrapS));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, translate(createInfo.wrapT));

	// set texture filtering parameters
	TextureMinFilter minFilter = createInfo.minFilter;
	if (!createInfo.mipmap)
	{
		if (createInfo.minFilter == TextureMinFilter::NearestMipmapNearest) minFilter = TextureMinFilter::Nearest;
		else if (createInfo.minFilter != TextureMinFilter::Nearest) minFilter = TextureMinFilter::Linear;
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, translate(minFilter));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, translate(createInfo.magFilter));

//
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//
//#define GL_TEXTURE_MAX_ANISOTROPY_EXT 0x84FE
//#define GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT 0x84FF
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, 16);


	// set texture format
	GLenum format = GL_RGB;
	GLint internalFormat = GL_RGB;
	GLenum oglType = GL_UNSIGNED_BYTE;
	getTextureFormatType(createInfo.format, GL_TEXTURE_2D, format, internalFormat, oglType);
		
	glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, createInfo.width, createInfo.height, 0, format, oglType, createInfo.data);
	if (createInfo.mipmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	// restore prev state
	glBindTexture(GL_TEXTURE_2D, currentTexture2D[0]);
	glPixelStorei(GL_UNPACK_ALIGNMENT, Alignment);
	return true;
}
//-----------------------------------------------------------------------------
bool Texture2D::CreateFromFiles(const Texture2DLoaderInfo& loaderInfo)
{
	Texture2DCreateInfo createInfo(loaderInfo);	
	{
		if (loaderInfo.verticallyFlip)
			stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

		int width = 0;
		int height = 0;
		int nrChannels = 0;
		TexelsFormat format = TexelsFormat::RGB_U8;
		uint8_t* data = stbi_load(loaderInfo.fileName.c_str(), &width, &height, &nrChannels, 0);
		if (!data || nrChannels < STBI_grey || nrChannels > STBI_rgb_alpha || width == 0 || height == 0)
		{
			LogError(("Texture loading failed! Filename='" + loaderInfo.fileName + "'").c_str());
			return false;
		}
		if (nrChannels == STBI_grey) format = TexelsFormat::R_U8;
		else if (nrChannels == STBI_grey_alpha) format = TexelsFormat::RG_U8;
		else if (nrChannels == STBI_rgb) format = TexelsFormat::RGB_U8;
		else if (nrChannels == STBI_rgb_alpha) format = TexelsFormat::RGBA_U8;

		createInfo.format = format;
		createInfo.width = static_cast<uint16_t>(width);
		createInfo.height = static_cast<uint16_t>(height);
		createInfo.depth = 1;
		createInfo.data = data;

		bool isValid = CreateFromMemories(createInfo);
		stbi_image_free((void*)data);
		if (!isValid) return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
void Texture2D::Destroy()
{
	if (id > 0)
	{
		for (unsigned i = 0; i < 8; i++)
		{
			if (currentTexture2D[i] == id)
				Texture2D::UnBind(i);
		}
		glDeleteTextures(1, &id);
		id = 0;
	}
}
//-----------------------------------------------------------------------------
void Texture2D::Bind(unsigned slot)
{
	if (currentTexture2D[slot] != id)
	{
		currentTexture2D[slot] = id;
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_2D, id);
	}
}
//-----------------------------------------------------------------------------
void Texture2D::UnBind(unsigned slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
	currentTexture2D[slot] = 0;
}
//=============================================================================
bool VertexBuffer::Create(RenderResourceUsage usage, unsigned vertexCount, unsigned vertexSize, const void* data)
{
	if (m_id > 0) Destroy();

	m_vertexCount = vertexCount;
	m_vertexSize = vertexSize;
	m_usage = usage;
	glGenBuffers(1, &m_id);

	GLint currentVBO = 0;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVBO);

	glBindBuffer(GL_ARRAY_BUFFER, m_id);
	glBufferData(GL_ARRAY_BUFFER, vertexCount * m_vertexSize, data, translate(m_usage));

	glBindBuffer(GL_ARRAY_BUFFER, static_cast<GLuint>(currentVBO));
	return true;
}
//-----------------------------------------------------------------------------
void VertexBuffer::Destroy()
{
	GLint currentVBO = 0;
	glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &currentVBO);
	if (static_cast<GLuint>(currentVBO) == m_id) glBindBuffer(GL_ARRAY_BUFFER, 0);
	if (m_id) glDeleteBuffers(1, &m_id);
	m_id = 0;
}
//-----------------------------------------------------------------------------
void VertexBuffer::Update(unsigned offset, unsigned size, const void* data)
{
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
//-----------------------------------------------------------------------------
void VertexBuffer::Bind() const
{
	glBindBuffer(GL_ARRAY_BUFFER, m_id);
}
//=============================================================================
bool IndexBuffer::Create(RenderResourceUsage usage, unsigned indexCount, unsigned indexSize, const void* data)
{
	if (m_id > 0) Destroy();

	m_indexCount = indexCount;
	m_indexSize = indexSize;
	m_usage = usage;
	glGenBuffers(1, &m_id);

	GLint currentIBO = 0;
	glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &currentIBO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * indexSize, data, translate(m_usage));

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, static_cast<GLuint>(currentIBO));
	return true;
}
//-----------------------------------------------------------------------------
void IndexBuffer::Destroy()
{
	glDeleteBuffers(1, &m_id);
	m_id = 0;
}
//-----------------------------------------------------------------------------
void IndexBuffer::Bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
}
//=============================================================================
inline GLenum translate(PrimitiveDraw p)
{
	switch (p)
	{
	case PrimitiveDraw::Lines:     return GL_LINES;
	case PrimitiveDraw::Triangles: return GL_TRIANGLES;
	case PrimitiveDraw::Points:    return GL_POINTS;
	}
	return 0;
}
//-----------------------------------------------------------------------------
bool VertexArrayBuffer::Create(VertexBuffer* vbo, IndexBuffer* ibo, const std::vector<VertexAttribute>& attribs)
{
	if (m_id > 0) Destroy();

	m_ibo = ibo;
	m_vbo = vbo;

	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);

	vbo->Bind();
	for (size_t i = 0; i < attribs.size(); i++)
	{
		const auto& att = attribs[i];
		glEnableVertexAttribArray(static_cast<GLuint>(i));
		glVertexAttribPointer(i, att.size, att.type, att.normalized ? GL_TRUE : GL_FALSE, static_cast<GLsizei>(att.stride), att.pointer);
	}

	if (m_ibo) m_ibo->Bind();

	glBindVertexArray(currentVAO);
	return true;
}
//-----------------------------------------------------------------------------
bool VertexArrayBuffer::Create(const std::vector<VertexBuffer*> vbo, IndexBuffer* ibo, const std::vector<VertexAttribute>& attribs)
{
	if (m_id > 0) Destroy();
	if (vbo.size() != attribs.size()) return false;

	m_ibo = ibo;
	m_vbo = vbo[0];

	glGenVertexArrays(1, &m_id);
	glBindVertexArray(m_id);

	for (size_t i = 0; i < vbo.size(); i++)
	{
		const auto& att = attribs[i];
		glEnableVertexAttribArray(i);
		vbo[i]->Bind();
		glVertexAttribPointer(i, att.size, att.type, att.normalized ? GL_TRUE : GL_FALSE, att.stride, att.pointer);
	}
	if (m_ibo) m_ibo->Bind();

	glBindVertexArray(currentVAO);
	return true;
}
//-----------------------------------------------------------------------------
void VertexArrayBuffer::Destroy()
{
	if (m_id > 0 && currentVAO == m_id)
	{
		if (m_ibo) glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		currentVAO = 0;
		glBindVertexArray(0);
		glDeleteVertexArrays(1, &m_id);
	}
	m_id = 0;
}
//-----------------------------------------------------------------------------
void VertexArrayBuffer::Draw(PrimitiveDraw primitive)
{
	if (currentVAO != m_id)
	{
		currentVAO = m_id;
		glBindVertexArray(m_id);
	}

	if (m_ibo)
	{
		const unsigned indexSizeType = m_ibo->GetIndexSize() == sizeof(uint32_t) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
		glDrawElements(translate(primitive), m_ibo->GetIndexCount(), indexSizeType, nullptr);
	}
	else
	{
		glDrawArrays(translate(primitive), 0, m_vbo->GetVertexCount());
	}
}
//-----------------------------------------------------------------------------
void VertexArrayBuffer::DrawElementsBaseVertex(PrimitiveDraw primitive, uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex)
{
	if (!m_ibo) return;

	if (currentVAO != m_id)
	{
		currentVAO = m_id;
		glBindVertexArray(m_id);
	}

	const unsigned indexSizeType = m_ibo->GetIndexSize() == sizeof(uint32_t) ? GL_UNSIGNED_INT : GL_UNSIGNED_SHORT;
	glDrawElementsBaseVertex(translate(primitive), indexCount, indexSizeType, (void*)(m_ibo->GetIndexSize() * baseIndex), baseVertex);
}
//-----------------------------------------------------------------------------
void VertexArrayBuffer::UnBind()
{
	currentVAO = 0;
	glBindVertexArray(0);
}
//=============================================================================
bool FrameBuffer::Create(int width, int height)
{
	m_width = width;
	m_height = height;
	glGenFramebuffers(1, &m_id);
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);

	glGenTextures(1, &m_texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
#if 1
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
#else
	glPixelStorei(GL_UNPACK_SWAP_BYTES, GL_TRUE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB5_A1, width, height, 0, GL_RGBA, GL_UNSIGNED_SHORT_1_5_5_5_REV, nullptr);
#endif
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // TODO: GL_LINEAR 
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	Texture2D::UnBind(); // TODO:

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_texColorBuffer, 0);

	glGenRenderbuffers(1, &m_rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LogError("Framebuffer is not complete!");
		return false;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	return true;
}
//-----------------------------------------------------------------------------
void FrameBuffer::Destroy()
{
	glDeleteTextures(1, &m_texColorBuffer);
	glDeleteRenderbuffers(1, &m_rbo);
	glDeleteFramebuffers(1, &m_id);
	m_id = 0;
}
//-----------------------------------------------------------------------------
void FrameBuffer::Bind(const glm::vec3& color)
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_id);
	glClearColor(color.x, color.y, color.z, 1.0f);
	glViewport(0, 0, m_width, m_height);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
}
//-----------------------------------------------------------------------------
void FrameBuffer::MainFrameBufferBind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//-----------------------------------------------------------------------------
void FrameBuffer::BindTextureBuffer()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texColorBuffer);
}
//-----------------------------------------------------------------------------