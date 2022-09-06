#include "stdafx.h"
#include "TradeLogic.h"
#include "GameLogic.h"
//-----------------------------------------------------------------------------
void TradeLogic::Start(GameLogic* logicMgr)
{
	m_logicMgr = logicMgr;
	puts("Trade Logic Start");
}
//-----------------------------------------------------------------------------
void TradeLogic::Stop()
{
}
//-----------------------------------------------------------------------------
void TradeLogic::Update(float deltaTime)
{
	m_logicMgr->SetLogicStage(GameLogicStage::Exploring);
}
//-----------------------------------------------------------------------------
void TradeLogic::Draw()
{
}
//-----------------------------------------------------------------------------
void TradeLogic::DrawUI()
{
}
//-----------------------------------------------------------------------------