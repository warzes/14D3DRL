#include "stdafx.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
bool IsEngineRun = true;
//-----------------------------------------------------------------------------
bool Engine::Init(const EngineCreateInfo& createInfo)
{
	InitLogFile(createInfo.LogFile);

	if (!m_window.Create(createInfo.Window))
		return false;

	if (!m_renderSystem.Create())
		return false;

	m_startTime = std::chrono::high_resolution_clock::now();

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
	// get delta time
	{
		const auto curTime = std::chrono::high_resolution_clock::now();
		m_frameTimeCurrent = std::chrono::duration_cast<std::chrono::microseconds>(curTime - m_startTime).count();
		m_frameTimeDelta = m_frameTimeCurrent - m_frameTimeLast;
		m_frameTimeLast = m_frameTimeCurrent;
		m_deltaTime = static_cast<float>(m_frameTimeDelta) * MicrosecondsToSeconds;
	}

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
	return m_deltaTime;
}
//-----------------------------------------------------------------------------