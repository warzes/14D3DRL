#pragma once

class GameLogic;

class GameSceneLogic
{
public:
	virtual ~GameSceneLogic() = default;

	virtual void Start(GameLogic* logicMgr) = 0;
	virtual void Stop() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual void DrawUI() = 0;

protected:
	GameLogic* m_logicMgr = nullptr;
};