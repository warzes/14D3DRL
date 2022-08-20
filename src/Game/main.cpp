#include "stdafx.h"
#include "Engine.h"
//-----------------------------------------------------------------------------
bool GameAppInit();
void GameAppUpdate();
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
				GameAppUpdate();
				GameAppFrame();
				engine.Frame();
			}
		}
		GameAppClose();
	}
	engine.Close();
}
//-----------------------------------------------------------------------------