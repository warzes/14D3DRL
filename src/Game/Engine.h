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

	float GetDeltaTime() const;

private:
	WindowSystem m_window;
	RenderDevice m_renderDevice;
};