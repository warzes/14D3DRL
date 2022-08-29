#include "stdafx.h"
#include "GameLogic.h"

void GameLogic::Update()
{
	if (m_logicStage == GameLogicStage::Exploring)
		m_exploring.Update();
	else if (m_logicStage == GameLogicStage::Battle)
		m_battle.Update();
}

void GameLogic::SetLogicStage(GameLogicStage stage)
{
	m_logicStage = stage;
	if (stage == GameLogicStage::Exploring)
	{
		m_exploring.Start(this);
	}
	if (stage == GameLogicStage::Battle)
	{
		m_battle.Start(this);
	}
}