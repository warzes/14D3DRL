#pragma once

enum class RenderResourceUsage
{
	Static,
	Dynamic,
	Stream,
};

//=============================================================================
// ShaderProgram
//=============================================================================

struct UniformVariable
{
	UniformVariable() = default;
	UniformVariable(GLint newId) { id = newId; }
	bool IsValid() const { return id > -1; }
	int id = -1;
};

// TODO: юниформы хранящие свой тип данных (и статус изменения)

class ShaderProgram
{
public:
	bool CreateFromMemories(const std::string& vertexShaderMemory, const std::string& fragmentShaderMemory);

	void Destroy();

	void Bind();

	static void UnBind();

	UniformVariable GetUniformVariable(const char* name);

	void SetUniform(UniformVariable var, int value);
	void SetUniform(UniformVariable var, float value);
	void SetUniform(UniformVariable var, float x, float y, float z);
	void SetUniform(UniformVariable var, const glm::vec2& v);
	void SetUniform(UniformVariable var, const glm::vec3& v);
	void SetUniform(UniformVariable var, const glm::vec4& v);
	void SetUniform(UniformVariable var, const glm::mat3& mat);
	void SetUniform(UniformVariable var, const glm::mat4& mat);

	void SetUniform(const std::string& name, int value);
	void SetUniform(const std::string& name, float value);
	void SetUniform(const std::string& name, float x, float y, float z);
	void SetUniform(const std::string& name, const glm::vec2& v);
	void SetUniform(const std::string& name, const glm::vec3& v);
	void SetUniform(const std::string& name, const glm::vec4& v);
	void SetUniform(const std::string& name, const glm::mat3& mat);
	void SetUniform(const std::string& name, const glm::mat4& mat);

	bool IsValid() const { return m_id > 0; }

private:
	GLuint createShader(GLenum type, const std::string& source) const;

	unsigned m_id = 0;
};

//=============================================================================
// Texture
//=============================================================================

enum class TextureMinFilter
{
	Nearest,
	Linear,
	NearestMipmapNearest,
	NearestMipmapLinear,
	LinearMipmapNearest,
	LinearMipmapLinear,
};

enum class TextureMagFilter
{
	Nearest,
	Linear,
};

enum class TextureWrapping
{
	Repeat,
	MirroredRepeat,
	Clamp,
};

enum class TexelsFormat
{
	None = 0,
	R_U8,
	RG_U8,
	RGB_U8,
	RGBA_U8,
	Depth_U16,
	DepthStencil_U16,
	Depth_U24,
	DepthStencil_U24,
};

struct Texture2DLoaderInfo
{
	RenderResourceUsage usage = RenderResourceUsage::Static;

	TextureMinFilter minFilter = TextureMinFilter::NearestMipmapNearest;
	TextureMagFilter magFilter = TextureMagFilter::Nearest;
	TextureWrapping wrapS = TextureWrapping::Repeat;
	TextureWrapping wrapT = TextureWrapping::Repeat;
	TextureWrapping wrapR = TextureWrapping::Repeat;

	std::string fileName;
	bool verticallyFlip = true;
	bool mipmap = true;
};

struct Texture2DCreateInfo
{
	Texture2DCreateInfo() = default;
	Texture2DCreateInfo(const Texture2DLoaderInfo& loaderInfo)
	{
		usage = loaderInfo.usage;
		minFilter = loaderInfo.minFilter;
		magFilter = loaderInfo.magFilter;
		wrapS = loaderInfo.wrapS;
		wrapT = loaderInfo.wrapT;
		wrapR = loaderInfo.wrapR;
		mipmap = loaderInfo.mipmap;
	}

	RenderResourceUsage usage = RenderResourceUsage::Static;

	TextureMinFilter minFilter = TextureMinFilter::NearestMipmapNearest;
	TextureMagFilter magFilter = TextureMagFilter::Nearest;
	TextureWrapping wrapS = TextureWrapping::Repeat;
	TextureWrapping wrapT = TextureWrapping::Repeat;
	TextureWrapping wrapR = TextureWrapping::Repeat;

	TexelsFormat format = TexelsFormat::RGBA_U8;
	uint16_t width = 1;
	uint16_t height = 1;
	uint16_t depth = 1;
	uint8_t* data = nullptr;
	unsigned mipMapCount = 1; // TODO: only compressed
	bool mipmap = true;
};

class Texture2D
{
public:
	bool CreateFromMemories(const Texture2DCreateInfo& createInfo);
	bool CreateFromFiles(const Texture2DLoaderInfo& loaderInfo);

	void Destroy();

	void Bind(unsigned slot = 0);

	static void UnBind(unsigned slot = 0);

	bool IsValid() const { return id > 0; }

	unsigned id = 0;
};

//=============================================================================
// VertexBuffer
//=============================================================================

class VertexBuffer
{
public:
	bool Create(RenderResourceUsage usage, unsigned vertexCount, unsigned vertexSize, const void* data);
	void Destroy();

	void Update(unsigned offset, unsigned size, const void* data);

	void Bind() const;

	unsigned GetVertexCount() const { return m_vertexCount; }

	bool IsValid() const { return m_id > 0; }

private:
	RenderResourceUsage m_usage = RenderResourceUsage::Static;
	unsigned m_id = 0;
	unsigned m_vertexCount = 0;
	unsigned m_vertexSize = 0;
};

//=============================================================================
// IndexBuffer
//=============================================================================

class IndexBuffer
{
public:
	bool Create(RenderResourceUsage usage, unsigned indexCount, unsigned indexSize, const void* data);
	void Destroy();

	void Bind() const;

	unsigned GetIndexCount() const { return m_indexCount; }
	unsigned GetIndexSize() const { return m_indexSize; }

	bool IsValid() const { return m_id > 0; }

private:
	RenderResourceUsage m_usage = RenderResourceUsage::Static;
	unsigned m_id = 0;
	unsigned m_indexCount = 0;
	unsigned m_indexSize = 0;
};

//=============================================================================
// VertexArrayBuffer
//=============================================================================

struct VertexAttribute
{
	unsigned size;
	GLenum type;
	bool normalized;
	unsigned stride;     // = sizeof Vertex
	const void* pointer; // (void*)offsetof(Vertex, tex_coord)}
};

enum class PrimitiveDraw
{
	Lines,
	Triangles,
	Points,
};

class VertexArrayBuffer
{
public:
	bool Create(VertexBuffer* vbo, IndexBuffer* ibo, const std::vector<VertexAttribute>& attribs);
	bool Create(const std::vector<VertexBuffer*> vbo, IndexBuffer* ibo, const std::vector<VertexAttribute>& attribs);
	void Destroy();

	void Draw(PrimitiveDraw primitive = PrimitiveDraw::Triangles);
	void DrawElementsBaseVertex(PrimitiveDraw primitive, uint32_t indexCount, uint32_t baseIndex, uint32_t baseVertex);

	bool IsValid() const { return m_id > 0; }

	static void UnBind();

private:
	GLuint m_id = 0;
	IndexBuffer* m_ibo = nullptr;
	VertexBuffer* m_vbo = nullptr;
};

//=============================================================================
// FrameBuffer
//=============================================================================
class FrameBuffer
{
public:
	bool Create(int width, int height);
	void Destroy();

	void Bind(const glm::vec3& color);

	void BindTextureBuffer();

	static void MainFrameBufferBind();

	bool IsValid() const { return m_id > 0 && m_texColorBuffer > 0 && m_rbo > 0; }

private:
	GLuint m_id = 0;
	GLuint m_texColorBuffer = 0;
	GLuint m_rbo = 0;
	int m_width = 0;
	int m_height = 0;
};