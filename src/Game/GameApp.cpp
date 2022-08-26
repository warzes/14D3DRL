#include "stdafx.h"
#include "Camera.h"
#include "TileMap.h"
#include "Offscreen.h"
#include "GameInput.h"
#include "FreeMove.h"
//-----------------------------------------------------------------------------
Camera camera;
TileMap tileMap;
Offscreen offscreen;
extern bool IsEngineRun;
//-----------------------------------------------------------------------------
bool GameAppInit()
{
	if (!tileMap.Init())
		return false;

	glm::vec3 playerPos = tileMap.GetStartPlayerPos();

	camera.SetPosition({ playerPos.x, 0.3f, playerPos.y });
	//camera.SetRotate(playerPos.z, 0.0f);

	if (!offscreen.Init())
		return false;

	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LESS);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_BACK);

	return true;
}
//-----------------------------------------------------------------------------
void GameAppUpdate(float deltaTime)
{
	if (IsKeyDown(Key::Esc))
	{
		IsEngineRun = false;
		return;
	}

	FreeMove(camera, tileMap.GetMapTileData(), deltaTime, true);

	camera.Update();
}
//-----------------------------------------------------------------------------
void GameAppFrame()
{
	offscreen.Bind();
	tileMap.Draw(camera);
	
	offscreen.DrawToScreen();
}
//-----------------------------------------------------------------------------
void GameAppClose()
{
	tileMap.Close();
	offscreen.Close();
}
//-----------------------------------------------------------------------------