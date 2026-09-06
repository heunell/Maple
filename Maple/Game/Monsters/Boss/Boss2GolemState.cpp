#include "pch.h"
#include "Boss2GolemState.h"
#include "Boss2Component.h"
#include "Boss2IdleState.h"
#include "Component/SpriteComponent.h"
#include "Core/GameEngine.h"
#include "Game/Map/Boss/LucidPhase2.h"
#include "Game/Monsters/MonsterStateMachine.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterComponent.h"
#include "UI/BossPatternNotice.h"
#include "World/Level.h"
#include "World/World.h"

#include <functional>
#include <random>

void Boss2GolemState::EnableGolem(const Ptr<Boss2Golem>& Golem)
{
	if (Golem)
	{
		Golem->SetPoolEnable(true);
	}
}

void Boss2GolemState::DisableGolem(const Ptr<Boss2Golem>& Golem)
{
	if (Golem)
	{
		Golem->SetPoolEnable(false);
	}
}

void Boss2GolemState::SpawnGolemPattern()
{
	if (_Spawned)
	{
		return;
	}

	Ptr<Boss2Component> Owner = Lock(_Owner);

	if (!Owner)
	{
		return;
	}

	Ptr<MonsterStateMachine> StateMachine = Owner->GetStateMachine();

	if (!StateMachine || StateMachine->GetCurrentState().get() != this)
	{
		return;
	}

	_Spawned = true;

	Ptr<MonsterBase> MonsterOwner = Owner->GetMonster();

	if (!MonsterOwner)
	{
		return;
	}

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!LevelOwner)
	{
		return;
	}

	std::vector<Ptr<Actor>> MapActors;

	LevelOwner->FindActors("Map.LucidPhase2", MapActors);

	for (const Ptr<Actor>& MapActor : MapActors)
	{
		Ptr<LucidPhase2> Map = Cast<Actor, LucidPhase2>(MapActor);

		if (!Map)
		{
			continue;
		}

		const std::vector<FPhase2Platform>& Platforms = Map->GetPlatforms();

		std::vector<int32> PlatformIndices;

		for (int32 Index = 0; Index < static_cast<int32>(Platforms.size()); ++Index)
		{
			const FPhase2Platform& Platform = Platforms[Index];

			if (!Platform.Sprite || !Platform.Collision)
			{
				continue;
			}

			if (Platform.State != ePhase2PlatformState::Active)
			{
				continue;
			}

			PlatformIndices.push_back(Index);
		}

		if (PlatformIndices.empty())
		{
			return;
		}

		std::random_device RandomDevice;

		std::mt19937 RandomEngine(RandomDevice());

		std::uniform_int_distribution<int32> PlatformDistribution(0, static_cast<int32>(PlatformIndices.size()) - 1);

		int32 PlatformIndex = PlatformIndices[PlatformDistribution(RandomEngine)];

		const FPhase2Platform& Platform = Platforms[PlatformIndex];

		FVector3D LandingPosition = Platform.Collision->GetWorldPosition();

		LandingPosition._y += Platform.Collision->GetBoxSize()._y * 0.5f;

		Ptr<Boss2Golem> Golem = _GolemPool.Acquire();

		if (!Golem)
		{
			return;
		}

		Golem->Start(This<Boss2GolemState>(), Map, PlatformIndex, LandingPosition);

		return;
	}
}

bool Boss2GolemState::Init(Ptr<Boss2Component> Owner)
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

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (!BossSprite)
	{
		return false;
	}

	BossSprite->AddAnimationSequence("LUCID_MOB_8880150.skill3", false);

	BossSprite->AddNotify("LUCID_MOB_8880150.skill3", _PatternData.SpawnFrame, this, &Boss2GolemState::SpawnGolemPattern);

	if (!_GolemPool.Configure(
		_PatternData.PoolMaxCount,
		std::bind_front(&Boss2GolemState::CreateGolem, this),
		std::bind_front(&Boss2GolemState::EnableGolem, this),
		std::bind_front(&Boss2GolemState::DisableGolem, this)))
	{
		return false;
	}

	return _GolemPool.Reserve(_PatternData.SpawnCount);
}

Ptr<Boss2Golem> Boss2GolemState::CreateGolem()
{
	Ptr<Boss2Component> Owner = Lock(_Owner);

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

	return LevelOwner->SpawnActor<Boss2Golem>("Boss2Golem", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));
}

void Boss2GolemState::Enter(Ptr<MonsterComponent> Monster)
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

	BossSprite->ChangeAnimation("LUCID_MOB_8880150.skill3");

	BossSprite->SetAnimationFrame(0);

	BossSprite->SetPlay("LUCID_MOB_8880150.skill3", true);
}

Ptr<MonsterState> Boss2GolemState::Tick(Ptr<MonsterComponent> Monster, float DeltaTime)
{
	if (!Monster || !_Spawned)
	{
		return nullptr;
	}

	Ptr<Boss2Component> BossController = Cast<MonsterComponent, Boss2Component>(Monster);

	if (!BossController)
	{
		return nullptr;
	}

	return BossController->GetIdleState();
}

void Boss2GolemState::Destroy()
{
	_GolemPool.ReleaseAll();

	_Owner.reset();

	MonsterState::Destroy();
}

void Boss2GolemState::ReleaseGolem(Ptr<Boss2Golem> Golem)
{
	if (Golem)
	{
		_GolemPool.Release(Golem);
	}
}

void Boss2GolemState::Reset()
{
	_GolemPool.ReleaseAll();

	_Spawned = false;
}