#pragma once

#include "ExploringLogic.h"
#include "BattleLogic.h"
#include "TradeLogic.h"
#include "ChestLogic.h"
#include "TrapLogic.h"

enum class GameLogicStage
{
	Exploring,
	Battle,
	Trade,
	Chest,
	Trap
};

class GameLogic
{
public:
	GameLogic();

	void SetLogicStage(GameLogicStage stage);

	void Update();
	void Draw();
	void DrawUI();

private:
	ExploringLogic m_exploring;
	BattleLogic m_battle;
	TradeLogic m_trade;
	ChestLogic m_chest;
	TrapLogic m_trap;
	GameSceneLogic* m_currentSceneLogic = nullptr;
	GameLogicStage m_logicStage = GameLogicStage::Exploring;
};