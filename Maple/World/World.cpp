#include "pch.h"
#include "World.h"
#include "Level.h"
#include "GameLevel.h"
#include "Object/Actor.h"

World::World()
{}

World::~World()
{}

void World::Init(const std::string& name)
{
    _CurLevel = CreateLevel<GameLevel>(name);
}

void World::Tick(float DeltaTime)
{
    if (_CurLevel)
    {
        _CurLevel->Tick(DeltaTime);
    }
}

void World::Collision(float DeltaTime)
{
    if (_CurLevel)
    {
        _CurLevel->Collision(DeltaTime);
    }
}

void World::Render(float DeltaTime)
{
    if (_CurLevel)
    {
        _CurLevel->Render(DeltaTime);
    }
}

void World::RenderUI(float DeltaTime)
{
    if (_CurLevel)
    {
        _CurLevel->RenderUI(DeltaTime);
    }
}

void World::Destroy()
{
    DESTROY(_CurLevel);

    DESTROY(_NextLevel);
}

Ptr<class Level> World::GetCurLevel() const
{
    return _CurLevel;
}

Ptr<class Actor> World::GetPlayer() const
{
    return Lock<Actor>(_Player);
}

void World::SetMainPlayer(Ptr<class Actor> Player)
{
    _Player = Player;
}
