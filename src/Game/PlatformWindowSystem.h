#pragma once

struct WindowSystemCreateInfo
{
	int Width = 1024;
	int Height = 768;
	const wchar_t* Title = L"Game";

	bool fullscreen = false;
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

float GetScreenAspect();