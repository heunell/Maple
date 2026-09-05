#pragma once

#include "Level.h"
#include <chrono>

class GameLevel : public Level
{
public:
	GameLevel();
	virtual ~GameLevel();
	GameLevel(const GameLevel&) = delete;
	GameLevel(GameLevel&&) = delete;
	GameLevel& operator=(const GameLevel&) = delete;
	GameLevel& operator=(GameLevel&&) = delete;

private:
	Ptr<class MapManager> _MapManager;

	int32 _BossDeathCount = 10;

	std::chrono::steady_clock::time_point _BossBattleStartTime;

	bool _BossBattleRunning = false;

public:
    virtual bool Init(const std::string& MapName);

    virtual void Destroy() override;

	Ptr<class MapManager> GetMapManager() const;

	void StartBossBattle();

	int32 GetBossDeathCount() const;

	int32 GetBossRemainingSeconds() const;
};

