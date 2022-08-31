#pragma once

#include "GameSceneLogic.h"

class ExploringLogic final : public GameSceneLogic
{
public:
	void Start(GameLogic* logicMgr) final;
	void Stop() final;
	void Update() final;
	void Draw() final;
	void DrawUI() final;

private:
	glm::vec3 m_oldPlayerPos;
	float m_dist = 0.0f;
};