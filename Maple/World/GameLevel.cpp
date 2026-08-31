#include "pch.h"
#include "GameLevel.h"
#include "MapManager.h"

GameLevel::GameLevel()
{}

GameLevel::~GameLevel()
{}

bool GameLevel::Init(const std::string& MapName)
{
    if (MapName.empty())
    {
        return false;
    }

    // Level의 기본 Manager와 Player, TestActor를 먼저 생성한다.
    if (!Level::Init(""))
    {
        return false;
    }

    _MapManager = New<MapManager>();

    if (!_MapManager)
    {
        return false;
    }

    if (!_MapManager->Init(This<Level>()))
    {
        return false;
    }

    if (!_MapManager->ChangeMap(MapName))
    {
        return false;
    }

    return true;
}

Ptr<class MapManager> GameLevel::GetMapManager() const
{
    return _MapManager;
}

void GameLevel::Destroy()
{
    DESTROY(_MapManager);

    Level::Destroy();
}