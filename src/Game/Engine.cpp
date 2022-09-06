#include "stdafx.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
extern void InitLogFile(const char* fileName);
extern void CloseLogFile();
//-----------------------------------------------------------------------------
bool IsEngineRun = true;
//-----------------------------------------------------------------------------
namespace
{
	constexpr float MicrosecondsToSeconds = 1.0f / 1000000.0f;
	std::chrono::steady_clock::time_point startTime;
	int64_t frameTimeCurrent = 0;
	int64_t frameTimeLast = 0;
	int64_t frameTimeDelta = 0;
	float deltaTime = 0.0f;
}
//-----------------------------------------------------------------------------
bool Engine::Init(const EngineCreateInfo& createInfo)
{
	InitLogFile(createInfo.LogFile);

	if (!m_window.Create(createInfo.Window))
		return false;

	if (!m_renderSystem.Create())
		return false;

	startTime = std::chrono::high_resolution_clock::now();

	return true;
}
//-----------------------------------------------------------------------------
void Engine::Close()
{
	m_textureManager.Destroy();
	m_renderSystem.Destroy();
	m_window.Destroy();
	CloseLogFile();
}
//-----------------------------------------------------------------------------
bool Engine::Update()
{
	const auto curTime = std::chrono::high_resolution_clock::now();
	frameTimeCurrent = std::chrono::duration_cast<std::chrono::microseconds>(curTime - startTime).count();
	frameTimeDelta = frameTimeCurrent - frameTimeLast;
	frameTimeLast = frameTimeCurrent;
	deltaTime = static_cast<float>(frameTimeDelta) * MicrosecondsToSeconds;

	return IsEngineRun && m_window.Update();
}
//-----------------------------------------------------------------------------
void Engine::BeginFrame()
{
	m_renderSystem.Clear();
}
//-----------------------------------------------------------------------------
void Engine::EndFrame()
{
	m_renderSystem.Swap();
}
//-----------------------------------------------------------------------------
float Engine::GetDeltaTime() const
{
	return deltaTime;
}
//-----------------------------------------------------------------------------