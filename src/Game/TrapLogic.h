#pragma once

#include "GameSceneLogic.h"

class TrapLogic final : public GameSceneLogic
{
public:
	void Start(GameLogic* logicMgr) final;
	void Stop() final;
	void Update() final;
	void Draw() final;
	void DrawUI() final;
};