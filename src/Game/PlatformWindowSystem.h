#pragma once

struct WindowSystemCreateInfo
{
	int Width = 1024;
	int Height = 768;
	const char* Title = "Game";

	bool HighDpi = false;
	bool Fullscreen = false;
};

class WindowSystem
{
public:
	WindowSystem();
	~WindowSystem();

	bool Create(const WindowSystemCreateInfo& createInfo);
	void Destroy();

	bool Update();
};
extern WindowSystem* gWindowSystem;

float GetWindowAspect();
int GetWindowWidth();
int GetWindowHeight();