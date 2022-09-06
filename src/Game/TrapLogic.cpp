#include "stdafx.h"
#include "TrapLogic.h"
#include "GameLogic.h"
//-----------------------------------------------------------------------------
void TrapLogic::Start(GameLogic* logicMgr)
{
	m_logicMgr = logicMgr;
	puts("Trap Logic Start");
}
//-----------------------------------------------------------------------------
void TrapLogic::Stop()
{
}
//-----------------------------------------------------------------------------
void TrapLogic::Update(float deltaTime)
{
	m_logicMgr->SetLogicStage(GameLogicStage::Exploring);
}
//-----------------------------------------------------------------------------
void TrapLogic::Draw()
{
}
//-----------------------------------------------------------------------------
void TrapLogic::DrawUI()
{
}
//-----------------------------------------------------------------------------