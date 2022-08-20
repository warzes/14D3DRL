#include "stdafx.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
extern void InitLogFile(const char* fileName);
extern void CloseLogFile();
//-----------------------------------------------------------------------------
bool IsEngineRun = true;
//-----------------------------------------------------------------------------
bool Engine::Init()
{
	InitLogFile("../log.txt");

	if (!m_window.Create({}))
		return false;

	return true;
}
//-----------------------------------------------------------------------------
void Engine::Close()
{
	m_window.Destroy();
	CloseLogFile();
}
//-----------------------------------------------------------------------------
bool Engine::Update()
{
	return IsEngineRun && m_window.Update();
}
//-----------------------------------------------------------------------------
void Engine::Frame()
{
}
//-----------------------------------------------------------------------------