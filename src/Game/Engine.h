#pragma once

#include "PlatformWindowSystem.h"
#include "RenderSystem.h"
#include "Render2D.h"
#include "TextureManager.h"

struct EngineCreateInfo
{
	WindowSystemCreateInfo Window;
	const char* LogFile = "../log.txt";
};

class Engine
{
public:
	bool Init(const EngineCreateInfo& createInfo);
	void Close();

	bool Update();
	void BeginFrame();
	void EndFrame();

	float GetDeltaTime() const;

private:
	WindowSystem m_window;
	RenderSystem m_renderSystem;
	TextureManager m_textureManager;
};