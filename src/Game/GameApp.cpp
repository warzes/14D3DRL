#include "stdafx.h"
#include "Camera.h"
#include "TileMap.h"
#include "Offscreen.h"
//-----------------------------------------------------------------------------
Camera camera;
TileMap tileMap;
Offscreen offscreen;
//-----------------------------------------------------------------------------
bool GameAppInit()
{
	camera.SetPosition({ 50.0f, 0.0f, 80.0f });

	if (!tileMap.Init())
		return false;

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
	camera.SimpleMove(deltaTime);
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