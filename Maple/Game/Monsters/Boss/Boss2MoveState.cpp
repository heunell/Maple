#include "pch.h"
#include "Boss2MoveState.h"
#include "Boss2Component.h"
#include "Boss2BlackBoard.h"
#include "Component/SpriteComponent.h"
#include "Game/Map/Boss/LucidPhase2.h"
#include "Game/Monsters/MonsterBase.h"
#include "Game/Monsters/MonsterComponent.h"
#include "World/Level.h"
#include <random>

bool Boss2MoveState::Init(Ptr<class MonsterComponent> Owner, Ptr<class MonsterState> IdleState)
{
	if (!Owner || !IdleState)
	{
		return false;
	}

	_Owner = Owner;

	_IdleState = IdleState;

	return true;
}

void Boss2MoveState::Enter(Ptr<MonsterComponent> Monster)
{
	_TargetSelected = false;

	_TargetPosition = FVector3D::Zero;

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

	if (BossSprite)
	{
		BossSprite->ChangeAnimation("LUCID_MOB_8880150.fly");

		BossSprite->SetAnimationFrame(0);

		BossSprite->SetPlay("LUCID_MOB_8880150.fly", true);
	}

	SelectTargetPlatform(Monster);
}

Ptr<MonsterState> Boss2MoveState::Tick(Ptr<class MonsterComponent> Monster, float DeltaTime)
{
	if (!Monster)
	{
		return nullptr;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return nullptr;
	}

	if (!_TargetSelected)
	{
		if (!SelectTargetPlatform(Monster))
		{
			return Lock(_IdleState);
		}
	}

	FVector3D CurrentPosition = MonsterOwner->GetWorldPosition();

	FVector3D Difference = _TargetPosition - CurrentPosition;

	Ptr<SpriteComponent> BossSprite = MonsterOwner->FindSceneComponent<SpriteComponent>("BossSprite");

	if (BossSprite && Difference._x != 0.f)
	{
		BossSprite->SetAnimationFlip(Difference._x > 0.f);
	}

	float Distance = Difference.Size();

	if (Distance <= 1.f)
	{
		MonsterOwner->SetWorldPosition(_TargetPosition);

		return Lock(_IdleState);
	}

	float MoveDistance = _PatternData.MoveSpeed * DeltaTime;

	if (MoveDistance >= Distance)
	{
		MonsterOwner->SetWorldPosition(_TargetPosition);

		return Lock(_IdleState);
	}

	FVector3D Direction = Difference / Distance;

	MonsterOwner->SetWorldPosition(CurrentPosition + Direction * MoveDistance);

	return nullptr;
}

void Boss2MoveState::Destroy()
{
	_Owner.reset();

	_IdleState.reset();

	MonsterState::Destroy();
}

bool Boss2MoveState::SelectTargetPlatform(Ptr<class MonsterComponent> Monster)
{
	if (!Monster)
	{
		return false;
	}

	Ptr<MonsterBase> MonsterOwner = Monster->GetMonster();

	if (!MonsterOwner)
	{
		return false;
	}

	Ptr<Level> LevelOwner = MonsterOwner->GetLevel();

	if (!LevelOwner)
	{
		return false;
	}

	std::vector<Ptr<Actor>> MapActors;

	LevelOwner->FindActors("Map.LucidPhase2", MapActors);

	std::vector<FVector3D> TargetPositions;

	FVector3D CurrentPosition = MonsterOwner->GetWorldPosition();

	for (const Ptr<Actor>& MapActor : MapActors)
	{
		Ptr<LucidPhase2> Map = Cast<Actor, LucidPhase2>(MapActor);

		if (!Map)
		{
			continue;
		}

		const std::vector<FPhase2Platform>& Platforms = Map->GetPlatforms();

		for (const FPhase2Platform& Platform : Platforms)
		{
			if (!Platform.Collision)
			{
				continue;
			}

			if (Platform.State != ePhase2PlatformState::Active)
			{
				continue;
			}

			FVector3D PlatformPosition = Platform.Collision->GetWorldPosition();

			float PlatformTop = PlatformPosition._y + Platform.Collision->GetBoxSize()._y * 0.5f;

			FVector3D TargetPosition = PlatformPosition;

			TargetPosition._y = PlatformTop + 60.f + 62.5f;

			if (CurrentPosition.Distance(TargetPosition) <= 1.f)
			{
				continue;
			}

			TargetPositions.push_back(TargetPosition);
		}
	}

	if (TargetPositions.empty())
	{
		return false;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> Distribution(0, static_cast<int32>(TargetPositions.size()) - 1);

	_TargetPosition = TargetPositions[Distribution(RandomEngine)];

	_TargetSelected = true;

	return true;
}
