#pragma once

#include "ExploringLogic.h"
#include "BattleLogic.h"

enum class GameLogicStage
{
	Exploring,
	Battle
};

class GameLogic
{
public:
	void Update();

	void SetLogicStage(GameLogicStage stage);

private:
	ExploringLogic m_exploring;
	BattleLogic m_battle;

	GameLogicStage m_logicStage = GameLogicStage::Exploring;
};