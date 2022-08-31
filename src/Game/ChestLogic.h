#pragma once

#include "GameSceneLogic.h"

class ChestLogic final : public GameSceneLogic
{
public:
	void Start(GameLogic* logicMgr) final;
	void Stop() final;
	void Update() final;
	void Draw() final;
	void DrawUI() final;
};