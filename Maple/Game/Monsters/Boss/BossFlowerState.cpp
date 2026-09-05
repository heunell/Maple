#include "pch.h"
#include "BossFlowerState.h"
#include "BossComponent.h"
#include "BossBlackBoard.h"
#include "BossIdleState.h"
#include "Game/Monsters/MonsterBase.h"
#include "World/Level.h"

#include <functional>
#include <random>

bool BossFlowerState::Init(Ptr<BossComponent> Owner)
{
	if (!Owner)
	{
		return false;
	}

	_Owner = Owner;

	if (!_FlowerPool.Configure(
		_PatternData.PoolMaxCount,
		std::bind_front(&BossFlowerState::CreateFlower, this),
		std::bind_front(&BossFlowerState::EnableFlower, this),
		std::bind_front(&BossFlowerState::DisableFlower, this)))
	{
		return false;
	}

	return _FlowerPool.Reserve(_PatternData.PoolMaxCount);
}

Ptr<BossFlower> BossFlowerState::CreateFlower()
{
	Ptr<BossComponent> BossController = Lock(_Owner);

	if (!BossController)
	{
		return nullptr;
	}

	Ptr<MonsterBase> MonsterOwner = BossController->GetMonster();

	if (!MonsterOwner)
	{
		return nullptr;
	}

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!LevelOwner)
	{
		return nullptr;
	}

	return LevelOwner->SpawnActor<BossFlower>("BossFlower", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
}

void BossFlowerState::EnableFlower(const Ptr<BossFlower>& Flower)
{
	Flower->SetPoolEnable(true);
}

void BossFlowerState::DisableFlower(const Ptr<BossFlower>& Flower)
{
	Flower->SetPoolEnable(false);
}

void BossFlowerState::Enter(Ptr<MonsterComponent> Monster)
{
	_Spawned = false;

	if (!Monster)
	{
		return;
	}

	SpawnFlowerPattern();
}

Ptr<MonsterState> BossFlowerState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
{
	if (!Monster)
	{
		return nullptr;
	}

	Ptr<BossComponent> BossController = Cast<MonsterComponent, BossComponent>(Monster);

	if (!BossController)
	{
		return nullptr;
	}

	if (!_Spawned)
	{
		return nullptr;
	}

	if (_FlowerPool.GetInUseCount() > 0)
	{
		return nullptr;
	}

	return BossController->GetIdleState();
}

void BossFlowerState::SpawnFlowerPattern()
{
	if (_Spawned)
	{
		return;
	}

	Ptr<BossComponent> BossController = Lock(_Owner);

	if (!BossController)
	{
		return;
	}

	Ptr<MonsterBlackBoard> MonsterBoard = BossController->GetBlackBoard();

	if (!MonsterBoard)
	{
		return;
	}

	Ptr<BossBlackBoard> BossBoard = Cast<MonsterBlackBoard, BossBlackBoard>(MonsterBoard);

	if (!BossBoard)
	{
		return;
	}

	FBossPatternAreaData& PatternArea = BossBoard->PatternArea;

	if (PatternArea.SlotCount <= 0)
	{
		return;
	}

	if (PatternArea.GroundSpawnPositions.size() <= static_cast<std::size_t>(_PatternData.LastSlotIndex))
	{
		return;
	}

	int32 FlowerCount = _PatternData.LastSlotIndex - _PatternData.FirstSlotIndex + 1;

	if (_FlowerPool.GetAvailableCount() < static_cast<std::size_t>(FlowerCount))
	{
		return;
	}

	float SlotWidth = (PatternArea.RightBound - PatternArea.LeftBound) / static_cast<float>(PatternArea.SlotCount);

	float JitterRange = SlotWidth * _PatternData.SpawnJitterRatio;

	int32 TotalWeight = _PatternData.MediumSmallWeight + _PatternData.MediumWeight + _PatternData.LargeWeight + _PatternData.ExtraLargeWeight;

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> SizeDistribution(1, TotalWeight);

	std::uniform_int_distribution<int32> DirectionDistribution(0, 1);

	std::uniform_real_distribution<float> RotationDistribution(_PatternData.RotationMin, _PatternData.RotationMax);

	std::uniform_real_distribution<float> PositionDistribution(-JitterRange, JitterRange);

	for (int32 Index = _PatternData.FirstSlotIndex; Index <= _PatternData.LastSlotIndex; ++Index)
	{
		int32 SizeRoll = SizeDistribution(RandomEngine);

		int32 ResourceIndex = 3;

		if (SizeRoll <= _PatternData.MediumWeight)
		{
			ResourceIndex = 1;
		}
		else if (SizeRoll <= _PatternData.MediumWeight + _PatternData.MediumSmallWeight)
		{
			ResourceIndex = 0;
		}
		else if (SizeRoll <= _PatternData.MediumWeight + _PatternData.MediumSmallWeight + _PatternData.LargeWeight)
		{
			ResourceIndex = 2;
		}

		float Rotation = RotationDistribution(RandomEngine);

		if (DirectionDistribution(RandomEngine) == 0)
		{
			Rotation = -Rotation;
		}

		FVector3D SpawnPosition = PatternArea.GroundSpawnPositions[Index];

		SpawnPosition._x += PositionDistribution(RandomEngine);

		Ptr<BossFlower> Flower = _FlowerPool.Acquire();

		if (!Flower)
		{
			_FlowerPool.ReleaseAll();

			_Spawned = true;

			return;
		}

		Flower->Start(This<BossFlowerState>(), SpawnPosition, ResourceIndex, Rotation);
	}

	_Spawned = true;
}

void BossFlowerState::ReleaseFlower(Ptr<BossFlower> Flower)
{
	if (!Flower)
	{
		return;
	}

	_FlowerPool.Release(Flower);
}

void BossFlowerState::Reset()
{
	_FlowerPool.ReleaseAll();

	_Spawned = false;
}

void BossFlowerState::Destroy()
{
	_FlowerPool.ReleaseAll();

	_Owner.reset();

	MonsterState::Destroy();
}