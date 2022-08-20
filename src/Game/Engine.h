#pragma once

#include "PlatformWindowSystem.h"
#include "RenderDevice.h"

class Engine
{
public:
	bool Init();
	void Close();

	bool Update();
	void BeginFrame();
	void EndFrame();

private:
	WindowSystem m_window;
	RenderDevice m_renderDevice;
};