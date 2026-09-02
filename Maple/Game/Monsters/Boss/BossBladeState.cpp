#include "pch.h"
#include "BossBladeState.h"
#include "Component/SpriteComponent.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterComponent.h"
#include "World/Level.h"

#include <cmath>
#include <functional>
#include <numbers>
#include <random>


bool BossBladeState::Init(Ptr<MonsterComponent> Owner, Ptr<MonsterState> IdleState, const FBossBladeStateData& StateData)
{
	if (!Owner || !IdleState || StateData.CastAnimation.empty() || StateData.IdleAnimation.empty())
	{
		return false;
	}

	_Owner = Owner;

	_IdleState = IdleState;

	_StateData = StateData;

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return false;
	}

	Ptr<SpriteComponent> BossSprite =
		MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (!BossSprite)
	{
		return false;
	}

	BossSprite->AddNotify(_StateData.CastAnimation, _StateData.SpawnFrame, this, &BossBladeState::SpawnBladePattern);

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
	Ptr<MonsterComponent> Owner = Lock(_Owner);

	if (!Owner)
	{
		return nullptr;
	}

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

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
	if (Blade)
	{
		Blade->SetPoolEnable(true);
	}
}

void BossBladeState::DisableBlade(const Ptr<BossBlade>& Blade)
{
	if (Blade)
	{
		Blade->SetPoolEnable(false);
	}
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

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (!BossSprite)
	{
		return;
	}

	BossSprite->ChangeAnimation(_StateData.CastAnimation);
}

Ptr<MonsterState> BossBladeState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
{
	if (!Monster || !_Spawned)
	{
		return nullptr;
	}

	if (_BladePool.GetInUseCount() > 0)
	{
		return nullptr;
	}

	return Lock(_IdleState);
}

void BossBladeState::SpawnBladePattern()
{
	if (_Spawned)
	{
		return;
	}

	Ptr<MonsterComponent> Owner = Lock(_Owner);

	if (!Owner)
	{
		return;
	}

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (!BossSprite)
	{
		return;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> CountDistribution(_PatternData.SpawnMinCount, _PatternData.SpawnMaxCount);

	std::uniform_int_distribution<int32> ResourceDistribution(0, _PatternData.ResourceCount - 1);

	std::uniform_real_distribution<float> AngleDistribution(0.f, std::numbers::pi_v<float> * 2.f);

	std::uniform_real_distribution<float> RadiusDistribution(0.f, _PatternData.SpawnMaxRadius);

	int32 BladeCount = CountDistribution(RandomEngine);

	if (_BladePool.GetAvailableCount() < static_cast<std::size_t>(BladeCount))
	{
		return;
	}

	FVector3D Center = MonsterOwner->GetWorldPosition() + _StateData.SpawnOffset;

	for (int32 Index = 0; Index < BladeCount; ++Index)
	{
		float Angle = AngleDistribution(RandomEngine);

		float Radius = RadiusDistribution(RandomEngine);

		FVector3D Direction(cosf(Angle), sinf(Angle), 0.f);

		FVector3D SpawnPosition(
			Center._x + Direction._x * Radius,
			Center._y + Direction._y * Radius,
			Center._z);

		Ptr<BossBlade> Blade = _BladePool.Acquire();

		if (!Blade)
		{
			return;
		}

		Blade->Start(This<BossBladeState>(), SpawnPosition, Direction, ResourceDistribution(RandomEngine));
	}

	_Spawned = true;

	BossSprite->ChangeAnimation(_StateData.IdleAnimation);
}

void BossBladeState::ReleaseBlade(Ptr<BossBlade> Blade)
{
	if (Blade)
	{
		_BladePool.Release(Blade);
	}
}

void BossBladeState::Destroy()
{
	_BladePool.ReleaseAll();

	_Owner.reset();

	_IdleState.reset();

	MonsterState::Destroy();
}