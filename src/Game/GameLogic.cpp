#include "stdafx.h"
#include "GameLogic.h"
//-----------------------------------------------------------------------------
GameLogic::GameLogic()
{
	m_currentSceneLogic = &m_exploring;
	m_currentSceneLogic->Start(this);
}
//-----------------------------------------------------------------------------
void GameLogic::SetLogicStage(GameLogicStage stage)
{
	m_currentSceneLogic->Stop();

	m_logicStage = stage;
	if (stage == GameLogicStage::Exploring)
		m_currentSceneLogic = &m_exploring;
	else if (stage == GameLogicStage::Battle)
		m_currentSceneLogic = &m_battle;
	else if (stage == GameLogicStage::Trade)
		m_currentSceneLogic = &m_trade;
	else if (stage == GameLogicStage::Chest)
		m_currentSceneLogic = &m_chest;
	else if (stage == GameLogicStage::Trap)
		m_currentSceneLogic = &m_trap;

	m_currentSceneLogic->Start(this);
}
//-----------------------------------------------------------------------------
void GameLogic::Update()
{
	m_currentSceneLogic->Update();
}
//-----------------------------------------------------------------------------
void GameLogic::Draw()
{
	m_currentSceneLogic->Draw();
}
//-----------------------------------------------------------------------------
void GameLogic::DrawUI()
{
	m_currentSceneLogic->DrawUI();
}
//-----------------------------------------------------------------------------