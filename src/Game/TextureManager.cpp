#include "stdafx.h"
#include "TextureManager.h"
#include "Log.h"
//-----------------------------------------------------------------------------
TextureManager* gTextureManager = nullptr;
//-----------------------------------------------------------------------------
TextureManager::TextureManager()
{
	assert(!gTextureManager);
	gTextureManager = this;
}
//-----------------------------------------------------------------------------
TextureManager::~TextureManager()
{
	gTextureManager = nullptr;
}
//-----------------------------------------------------------------------------
void TextureManager::Destroy()
{
	for (auto it = m_textures.begin(); it != m_textures.end(); ++it)
		it->second.Destroy();

	m_textures.clear();
}
//-----------------------------------------------------------------------------
Texture2D* TextureManager::GetTexture2D(const std::string& name)
{
	auto it = m_textures.find(name);
	if (it != m_textures.end())
	{
		return &it->second;
	}
	else
	{
		LogPrint(("Load texture: " + name).c_str());


		Texture2DLoaderInfo textureLoaderInfo = {};
		textureLoaderInfo.fileName = name;
		//textureLoaderInfo.mipmap = false;
		
		Texture2D texture;
		if (!texture.CreateFromFiles(textureLoaderInfo) || !texture.IsValid())
			return nullptr;

		m_textures[name] = texture;
		return &m_textures[name];		
	}
}
//-----------------------------------------------------------------------------