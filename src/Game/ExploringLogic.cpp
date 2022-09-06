#include "stdafx.h"
#include "ExploringLogic.h"
#include "GameLogic.h"
#include "Camera.h"
#include "TileMap.h"
#include "FreeMove.h"

extern Camera camera;
extern TileMap tileMap;

void ExploringLogic::Start(GameLogic* logicMgr)
{
	m_logicMgr = logicMgr;
	m_dist = 0.0f;
	m_oldPlayerPos = camera.GetPosition();

	puts("Exploring Logic Start");
}

void ExploringLogic::Stop()
{
}

void ExploringLogic::Update(float deltaTime)
{
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

	m_dist += glm::distance(camera.GetPosition(), m_oldPlayerPos);
	m_oldPlayerPos = camera.GetPosition();

	if (m_dist > 5.0f) // 5 метра
	{
		puts(std::to_string(m_dist).c_str());
		m_dist = 0.0f;

		int r = rand() % 10;
		if (r < 3)
			m_logicMgr->SetLogicStage(GameLogicStage::Battle);
	}
}

void ExploringLogic::Draw()
{
}

void ExploringLogic::DrawUI()
{
}