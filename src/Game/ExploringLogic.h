#pragma once

class GameLogic;

class ExploringLogic
{
public:
	void Start(GameLogic* logicMgr);

	void Update();

private:
	GameLogic* m_logicMgr = nullptr;
	glm::vec3 m_oldPlayerPos;

	float m_dist = 0.0f;
};