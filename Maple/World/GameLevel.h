#pragma once

#include "Level.h"

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

public:
    virtual bool Init(const std::string& MapName);

    virtual void Destroy() override;

	Ptr<class MapManager> GetMapManager() const;
};

