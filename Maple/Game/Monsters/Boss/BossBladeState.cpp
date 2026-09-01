#include "pch.h"
#include "BossBladeState.h"
#include "Boss.h"
#include "BossComponent.h"
#include "BossIdleState.h"
#include "Component/SpriteComponent.h"
#include "Game/Monsters/MonsterBase.h"
#include "World/Level.h"

#include <cmath>
#include <functional>
#include <numbers>
#include <random>

bool BossBladeState::Init(Ptr<BossComponent> Owner)
{
	if (!Owner)
	{
		return false;
	}

	_Owner = Owner;

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return false;
	}

	Ptr<Boss> BossOwner = Cast<MonsterBase, Boss>(MonsterOwner);

	if (!BossOwner)
	{
		return false;
	}

	Ptr<SpriteComponent> BossSprite = BossOwner->GetBossSprite();

	if (!BossSprite)
	{
		return false;
	}

	BossSprite->AddNotify("LUCID_MOB_8880140.skill1", _PatternData.SpawnFrame, this, &BossBladeState::SpawnBladePattern);

	if (!_BladePool.Configure(
		_PatternData.PoolMaxCount,
		std::bind_front(&BossBladeState::CreateBlade, this),
		std::bind_front(&BossBladeState::EnableBlade, this),
		std::bind_front(&BossBladeState::DisableBlade, this)))
	{
		return false;
	}

	return _BladePool.Reserve(_PatternData.PoolMaxCount);
}

Ptr<BossBlade> BossBladeState::CreateBlade()
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

	return LevelOwner->SpawnActor<BossBlade>("BossBlade", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
}

void BossBladeState::EnableBlade(const Ptr<BossBlade>& Blade)
{
	Blade->SetPoolEnable(true);
}

void BossBladeState::DisableBlade(const Ptr<BossBlade>& Blade)
{
	Blade->SetPoolEnable(false);
}

void BossBladeState::Enter(Ptr<MonsterComponent> Monster)
{
	_Spawned = false;

	if (!Monster)
	{
		return;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	Ptr<Boss> BossOwner = Cast<MonsterBase, Boss>(MonsterOwner);

	if (!BossOwner)
	{
		return;
	}

	Ptr<SpriteComponent> BossSprite = BossOwner->GetBossSprite();

	if (!BossSprite)
	{
		return;
	}

	BossSprite->ChangeAnimation("LUCID_MOB_8880140.skill1");
}

Ptr<MonsterState> BossBladeState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
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

	if (_BladePool.GetInUseCount() > 0)
	{
		return nullptr;
	}

	return BossController->GetIdleState();
}

void BossBladeState::SpawnBladePattern()
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

	Ptr<MonsterBase> MonsterOwner = BossController->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	Ptr<Boss> BossOwner = Cast<MonsterBase, Boss>(MonsterOwner);

	if (!BossOwner)
	{
		return;
	}

	Ptr<SpriteComponent> BossSprite = BossOwner->GetBossSprite();

	if (!BossSprite)
	{
		return;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> CountDistribution(_PatternData.SpawnMinCount,_PatternData.SpawnMaxCount);

	std::uniform_int_distribution<int32> ResourceDistribution(0, _PatternData.ResourceCount - 1);

	std::uniform_real_distribution<float> AngleDistribution(0.f, std::numbers::pi_v<float> *2.f);

	std::uniform_real_distribution<float> RadiusDistribution(0.f, _PatternData.SpawnMaxRadius);

	int32 BladeCount = CountDistribution(RandomEngine);

	if (_BladePool.GetAvailableCount() < static_cast<std::size_t>(BladeCount))
	{
		return;
	}

	FVector3D Center = MonsterOwner->GetWorldPosition() + _PatternData.SpawnOffset;

	for (int32 Index = 0; Index < BladeCount; ++Index)
	{
		float Angle = AngleDistribution(RandomEngine);

		float Radius = RadiusDistribution(RandomEngine);

		FVector3D Direction(cosf(Angle), sinf(Angle), 0.f);

		FVector3D SpawnPosition(Center._x + Direction._x * Radius, Center._y + Direction._y * Radius, Center._z);

		Ptr<BossBlade> Blade = _BladePool.Acquire();

		if (!Blade)
		{
			return;
		}

		Blade->Start(This<BossBladeState>(), SpawnPosition, Direction, ResourceDistribution(RandomEngine));
	}

	_Spawned = true;

	BossSprite->ChangeAnimation("LUCID_MOB_8880140.stand");
}

void BossBladeState::ReleaseBlade(Ptr<BossBlade> Blade)
{
	if (!Blade)
	{
		return;
	}

	_BladePool.Release(Blade);
}

void BossBladeState::Destroy()
{
	_BladePool.ReleaseAll();

	_Owner.reset();

	MonsterState::Destroy();
}