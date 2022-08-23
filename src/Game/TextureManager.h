#pragma once

#include "RenderResource.h"

class TextureManager
{
public:
	TextureManager();
	~TextureManager();

	void Destroy();

	Texture2D* GetTexture2D(const std::string& name);

private:
	std::unordered_map<std::string, Texture2D> m_textures;
};

extern TextureManager* gTextureManager;