#include "stdafx.h"
#include "BattleLogic.h"
#include "GameLogic.h"
//-----------------------------------------------------------------------------
void BattleLogic::Start(GameLogic* logicMgr)
{
	m_logicMgr = logicMgr;
	puts("Battle Logic Start");
}
//-----------------------------------------------------------------------------
void BattleLogic::Stop()
{
}
//-----------------------------------------------------------------------------
void BattleLogic::Update(float deltaTime)
{
	m_logicMgr->SetLogicStage(GameLogicStage::Exploring);
}
//-----------------------------------------------------------------------------
void BattleLogic::Draw()
{
}
//-----------------------------------------------------------------------------
void BattleLogic::DrawUI()
{
}
//-----------------------------------------------------------------------------