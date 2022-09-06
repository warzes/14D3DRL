#include "stdafx.h"
#include "Camera.h"
#include "TileMap.h"
#include "Offscreen.h"
#include "GameInput.h"
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
	camera.SetRotate(playerPos.z, 0.0f);

	if (!offscreen.Init())
		return false;

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

	gameLogic.Update(deltaTime);
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