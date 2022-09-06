#pragma once

class RenderSystem
{
public:
	RenderSystem();
	~RenderSystem();

	bool Create();
	void Destroy();

	void Clear();
	void Swap();
};

extern RenderSystem* gRenderSystem;