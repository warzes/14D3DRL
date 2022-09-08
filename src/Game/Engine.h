#pragma once

#include "PlatformWindowSystem.h"
#include "RenderSystem.h"
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

	// time
	constexpr static float MicrosecondsToSeconds = 1.0f / 1000000.0f;
	std::chrono::steady_clock::time_point m_startTime;
	int64_t m_frameTimeCurrent = 0;
	int64_t m_frameTimeLast = 0;
	int64_t m_frameTimeDelta = 0;
	float m_deltaTime = 0.0f;
};