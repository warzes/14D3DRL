#include "stdafx.h"
#include "Camera.h"
#include "TileMap.h"
#include "Offscreen.h"
#include "GameInput.h"
#include "FreeMove.h"
#include "GameLogic.h"
#include "GameUI.h"
//-----------------------------------------------------------------------------
Camera camera;
TileMap tileMap;
Offscreen offscreen;
extern bool IsEngineRun;
GameLogic gameLogic;
GameUI gameUI;
//-----------------------------------------------------------------------------
bool GameAppInit()
{
	if (!tileMap.Init())
		return false;

	if (!gameUI.Init())
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

	gameLogic.SetLogicStage(GameLogicStage::Exploring);

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

	// set tile visible
	const int px = static_cast<int>(floor(camera.GetPosition().x));
	const int py = static_cast<int>(floor(camera.GetPosition().z));

	for (int x = px - 4; x < px + 4; x++)
	{
		if (x < 0 || x >= SizeMap) continue;
		for (int y = py - 4; y < py + 4; y++)
		{
			if (y < 0 || y >= SizeMap) continue;
			tileMap.SetTileVisible(x, y);
		}
	}

	gameLogic.Update();
}
//-----------------------------------------------------------------------------
void GameAppFrame()
{
	offscreen.Bind();
	{
		tileMap.Draw(camera);
		gameLogic.Draw();
	}
	offscreen.DrawToScreen();

	gameUI.Draw(camera.GetPosition(), tileMap);
	gameLogic.DrawUI();
}
//-----------------------------------------------------------------------------
void GameAppClose()
{
	tileMap.Close();
	gameUI.Close();
	offscreen.Close();
}
//-----------------------------------------------------------------------------