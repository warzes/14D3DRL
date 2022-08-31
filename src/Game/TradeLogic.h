#pragma once

#include "GameSceneLogic.h"

class TradeLogic final : public GameSceneLogic
{
public:
	void Start(GameLogic* logicMgr) final;
	void Stop() final;
	void Update() final;
	void Draw() final;
	void DrawUI() final;
};