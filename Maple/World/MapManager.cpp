#include "pch.h"
#include "MapManager.h"
#include "Level.h"
#include "Object/Actor.h"
#include "Game/Monsters/Boss/Boss.h"
#include "Game/Map/Boss/LucidEntry.h"
#include "Game/Map/Boss/LucidPhase1.h"
#include "Game/Map/Boss/LucidPhase2.h"
#include "Component/MovementComponent.h"
#include "Core/GameEngine.h"
#include "World.h"
#include <vector>
MapManager::MapManager()
{}

MapManager::~MapManager()
{}

void MapManager::SetMapActorEnable(const std::string& MapName, bool Enable)
{
	Ptr<Level> OwnerLevel = Lock<Level>(_OwnerLevel);

	if (!OwnerLevel)
	{
		return;
	}

	std::vector<Ptr<Actor>> MapActors;

	OwnerLevel->FindActors("Map." + MapName, MapActors);

	for (Ptr<Actor>& MapActor : MapActors)
	{
		if (MapActor)
		{
			Ptr<Boss> BossActor = Cast<Actor, Boss>(MapActor);

			if (BossActor)
			{
				BossActor->ResetBattle();
			}

			MapActor->SetEnable(Enable);
		}
	}
}

bool MapManager::Init(Ptr<class Level> OwnerLevel)
{
	if (!OwnerLevel)
	{
		return false;
	}

	_OwnerLevel = OwnerLevel;

	_PlayerSpawnPositions =					// 캐릭터 맵 시작 스폰좌표는 앞으로 여기서 관리하기
	{
		{
			"LucidEntry", FVector3D(37.f, -248.f, 1.f)
		},
		{
			"LucidReward", FVector3D(37.f, -248.f, 1.f)
		},
		{
			"LucidPhase1", FVector3D(-423.f, -261.f, 1.f)
		},
		{
			"LucidPhase2", FVector3D(911.f, -967.f, 1.f)
		}
	};

	return true;
}

bool MapManager::ChangeMap(const std::string& MapName)
{
	Ptr<Level> OwnerLevel = Lock<Level>(_OwnerLevel);

	if (!OwnerLevel)
	{
		return false;
	}

	auto SpawnIt = _PlayerSpawnPositions.find(MapName);

	if (SpawnIt == _PlayerSpawnPositions.end())
	{
		return false;
	}

	Ptr<World> CurrentWorld = GameEngine::Instance().GetWorld();

	if (!CurrentWorld)
	{
		return false;
	}

	Ptr<Actor> Player = CurrentWorld->GetPlayer();

	if (!Player)
	{
		return false;
	}

	Ptr<MovementComponent> Movement = Player->FindActorComponent<MovementComponent>("Movement");

	if (!Movement)
	{
		return false;
	}

	Ptr<Actor> TargetMap;

	auto It = _Maps.find(MapName);

	if (It != _Maps.end())
	{
		TargetMap = Lock<Actor>(It->second);
	}

	// 아직 생성되지 않은 맵에만 Level 에 생성을 요청하기
	if (!TargetMap)
	{
		if (MapName == "LucidEntry")
		{
			TargetMap = OwnerLevel->SpawnActor<LucidEntry>("LucidEntry", FVector3D(0.f, 0.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
		}
		else if (MapName == "LucidPhase1")
		{
			TargetMap = OwnerLevel->SpawnActor<LucidPhase1>("LucidPhase1", FVector3D(0.f, 0.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
		}
		else if (MapName == "LucidPhase2")
		{
			TargetMap = OwnerLevel->SpawnActor<LucidPhase2>("LucidPhase2", FVector3D(0.f, 0.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
		}
		else if (MapName == "LucidReward")
		{
			TargetMap = OwnerLevel->SpawnActor<LucidEntry>("LucidReward", FVector3D(0.f, 0.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), eRoomType::Reward);
		}
		else
		{
			return false;
		}

		if (!TargetMap)
		{
			return false;
		}

		_Maps[MapName] = TargetMap;
	}

	Ptr<Actor> CurrentMap = Lock<Actor>(_CurrentMap);

	if (CurrentMap && CurrentMap != TargetMap)
	{
		SetMapActorEnable(CurrentMap->GetName(), false);
	}

	SetMapActorEnable(MapName, true);

	_CurrentMap = TargetMap;

	Player->SetWorldPosition(SpawnIt->second);

	Movement->ResetMovement();

	return true;
}

void MapManager::Destroy()
{
	// Map Actor는 Level 이 소유하니까 여기서 Destroy 안함
	_CurrentMap.reset();
	
	_Maps.clear();

	_Maps.clear();

	_PlayerSpawnPositions.clear();

	_OwnerLevel.reset();
}
