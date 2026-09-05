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

    if (MapName == "LucidPhase1" || MapName == "LucidPhase2")
    {
        StartBossBattle();
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

void GameLevel::StartBossBattle()
{
    // 새 보스전 입장에서만 호출한다.
    _BossDeathCount = 10;

    _BossBattleStartTime = std::chrono::steady_clock::now();

    _BossBattleRunning = true;
}

int32 GameLevel::GetBossDeathCount() const
{
    return _BossDeathCount;
}

int32 GameLevel::GetBossRemainingSeconds() const
{
    if (!_BossBattleRunning)
    {
        return 1200;
    }

    auto Elapsed = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::steady_clock::now() - _BossBattleStartTime);

    if (Elapsed >= std::chrono::seconds(1200)) // 레이드 시간 20분 = 1200초
    {
        return 0;
    }

    return 1200 - static_cast<int32>(Elapsed.count());
}

void GameLevel::Destroy()
{
    DESTROY(_MapManager);

    Level::Destroy();
}