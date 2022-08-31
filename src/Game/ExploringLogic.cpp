#include "stdafx.h"
#include "ExploringLogic.h"
#include "GameLogic.h"
#include "Camera.h"

extern Camera camera;

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

void ExploringLogic::Update()
{
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