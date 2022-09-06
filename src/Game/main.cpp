#include "stdafx.h"
#include "Engine.h"
#include "MathCore.h"
//-----------------------------------------------------------------------------
#if defined(_MSC_VER)
#	pragma comment( lib, "OpenGL32.lib" )
#endif
//-----------------------------------------------------------------------------
//https://en.uesp.net/wiki/Arena:Base_Weapons
//https://store.steampowered.com/app/1845670/Islands_of_the_Caliph/
//https://www.youtube.com/watch?v=kXcER1fi8KQ
//
//https://oris-romero.itch.io/pixeltools-pixelator
//https://yellowafterlife.itch.io/pixelfont
//https://matt-lucas.itch.io/level-buddy
//https://led.itch.io/tilemancer
//https://www.youtube.com/channel/UCDExQH9Tfc8DOy8AAmUb2ZQ/videos
// https://www.youtube.com/watch?v=kXcER1fi8KQ
//https://github.com/warzes/TinyDungeons и ниже
bool GameAppInit();
void GameAppUpdate(float deltaTime);
void GameAppFrame();
void GameAppClose();
//-----------------------------------------------------------------------------
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
{
	Engine engine;

	if (engine.Init({}))
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