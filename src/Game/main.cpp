#include "stdafx.h"
#include "Engine.h"
#include "CoreMath.h"
//ProceduralMapGenerator

//https://github.com/davemoore22/sorcery
//https://www.youtube.com/channel/UCEkmGGFyxu5vH6xAHej3Dag/videos
//https://github.com/TheFakeMontyOnTheRun/dungeons-of-noudar
//https://github.com/afritz1/OpenTESArena
//https://www.youtube.com/watch?v=4siUM7CmQ0M&list=WL&index=230&t=154s
//https://www.youtube.com/channel/UCfNlxHHR7Ucwspu_p9mepzw/videos
//https://github.com/erkkah/tigr/blob/master/src/tigr_win.c
//https://github.com/sentrip/tinyvk/tree/master/src
//https://github.com/laleksic/tiny_csg
//https://github.com/SFML/SFML
//https://zeux.io/2019/01/17/is-c-fast/
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
	//float exponent = 1.5f;
	//float baseXP = 100.0;

	//for (int i = 0; i < 30; i++)
	//{
	//	int t = floor(baseXP * pow((float)i, exponent));
	//	puts(std::to_string(t).c_str());
	//}

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