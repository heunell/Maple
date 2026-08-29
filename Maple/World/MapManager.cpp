#include "pch.h"
#include "MapManager.h"
#include "Level.h"
#include "Object/Actor.h"
#include "Game/Map/Boss/LucidEntry.h"
#include "Game/Map/Boss/LucidPhase1.h"

MapManager::MapManager()
{}

MapManager::~MapManager()
{}

bool MapManager::Init(Ptr<class Level> OwnerLevel)
{
	if (!OwnerLevel)
	{
		return false;
	}

	_OwnerLevel = OwnerLevel;

	return true;
}

bool MapManager::ChangeMap(const std::string& MapName)
{
	Ptr<Level> OwnerLevel = Lock<Level>(_OwnerLevel);

	if (!OwnerLevel)
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
		else if(MapName == "LucidPhase1")
		{
			TargetMap = OwnerLevel->SpawnActor<LucidPhase1>("LucidPhase1", FVector3D(0.f, 0.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
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
		// 기존 맵은 제거하지 않고 비활성화 처리하기
		CurrentMap->SetEnable(false);
	}

	TargetMap->SetEnable(true);

	_CurrentMap = TargetMap;


	return true;
}

void MapManager::Destroy()
{
	// Map Actor는 Level 이 소유하니까 여기서 Destroy 안함
	_CurrentMap.reset();
	
	_Maps.clear();

	_OwnerLevel.reset();
}
