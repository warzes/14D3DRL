#pragma once

#include "PlatformWindowSystem.h"

class Engine
{
public:
	bool Init();
	void Close();

	bool Update();
	void Frame();

private:
	WindowSystem m_window;
};