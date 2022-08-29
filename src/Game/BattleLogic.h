#pragma once

class GameLogic;

class BattleLogic
{
public:
	void Start(GameLogic* logicMgr);

	void Update();

private:
	GameLogic* m_logicMgr = nullptr;
};