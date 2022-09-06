#include "stdafx.h"
#include "ChestLogic.h"
#include "GameLogic.h"
//-----------------------------------------------------------------------------
void ChestLogic::Start(GameLogic* logicMgr)
{
	m_logicMgr = logicMgr;
	puts("Chest Logic Start");
}
//-----------------------------------------------------------------------------
void ChestLogic::Stop()
{
}
//-----------------------------------------------------------------------------
void ChestLogic::Update(float deltaTime)
{
	m_logicMgr->SetLogicStage(GameLogicStage::Exploring);
}
//-----------------------------------------------------------------------------
void ChestLogic::Draw()
{
}
//-----------------------------------------------------------------------------
void ChestLogic::DrawUI()
{
}
//-----------------------------------------------------------------------------