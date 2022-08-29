#include "stdafx.h"
#include "BattleLogic.h"
#include "GameLogic.h"

void BattleLogic::Start(GameLogic* logicMgr)
{
	m_logicMgr = logicMgr;
	puts("Battle Logic Start");
}

void BattleLogic::Update()
{
	m_logicMgr->SetLogicStage(GameLogicStage::Exploring);
}