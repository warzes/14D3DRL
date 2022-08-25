#include "stdafx.h"
#include "Engine.h"
#include "CoreMath.h"
ProceduralMapGenerator
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#	pragma comment( lib, "OpenGL32.lib" )
#endif
//-----------------------------------------------------------------------------
bool GameAppInit();
void GameAppUpdate(float deltaTime);
void GameAppFrame();
void GameAppClose();
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	Engine engine;

	if (engine.Init())
	{
		if (GameAppInit())
		{
			while (engine.Update())
			{
				GameAppUpdate(engine.GetDeltaTime());
				engine.BeginFrame();
				GameAppFrame();
				engine.EndFrame();
			}
		}
		GameAppClose();
	}
	engine.Close();
}
//-----------------------------------------------------------------------------