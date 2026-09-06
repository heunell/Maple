#include "pch.h"
#include "Boss2.h"
#include "Boss2Component.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "World/GameLevel.h"
#include "World/MapManager.h"

bool Boss2::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!MonsterBase::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("Monster");

	AddTag("Boss");

	_Status.MaxHP = 1000000000;

	_Status.CurrentHP = _Status.MaxHP;

	if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Monsters\\Lucid\\Phase2\\LucidPhase2.json"), "Animations"))
	{
		return false;
	}

	if (!ANIMATION_MANAGER->FindAnimation("LUCID_MOB_8880140.stand"))
	{
		if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Monsters\\Lucid\\Phase1\\LucidPhase1.json"), "Animations"))
		{
			return false;
		}
	}

	Ptr<Animation2DData> FlyData = ANIMATION_MANAGER->FindAnimation("LUCID_MOB_8880150.fly");

	if (!FlyData)
	{
		return false;
	}

	_BossSprite = CreateSceneComponent<SpriteComponent>("BossSprite");

	if (!_BossSprite)
	{
		return false;
	}

	_BossSprite->SetRenderLayerName("Default");

	_BossSprite->AddAnimationSequence(FlyData, true);

	_BossSprite->AddAnimationSequence("LUCID_MOB_8880150.die1"  , false);   // DIE

	_BossSprite->AddAnimationSequence("LUCID_MOB_8880150.skill1", false);	// 칼날

	_BossSprite->AddAnimationSequence("LUCID_MOB_8880150.skill2", false);	// 브레스

	_BossSprite->AttachToComponent(GetRoot());

	Ptr<AABBCollisionComponent> Collision = CreateSceneComponent<AABBCollisionComponent>("BossCollision");

	if (!Collision)
	{
		return false;
	}

	Collision->SetBoxSize(90.f, 125.f);

	Collision->SetRelativePosition(0.f, 60.f, 0.f);
	
	Collision->AttachToComponent(GetRoot());
	
	Collision->SetCollisionProfile("Boss");

	Ptr<Boss2Component> Component = CreateActorComponent<Boss2Component>("Boss2");

	if (!Component)
	{
		return false;
	}

	return true;
}

void Boss2::Tick(float DeltaTime)
{
	if (_Status.CurrentHP > 0)
	{
		MonsterBase::Tick(DeltaTime);

		return;
	}

	FindSceneComponent<AABBCollisionComponent>("BossCollision")->SetEnable(false);

	_BossSprite->ChangeAnimation("LUCID_MOB_8880150.die1");

	GetRoot()->Tick(DeltaTime);

	if (!_BossSprite->GetAnimation()->IsFinished())
	{
		return;
	}

	if (Ptr<GameLevel> CurrentLevel = Cast<Level, GameLevel>(GetLevel()))
	{
		if (CurrentLevel->GetMapManager())
		{
			CurrentLevel->GetMapManager()->ChangeMap("LucidReward");
		}
	}
}
Ptr<SpriteComponent> Boss2::GetBossSprite() const
{
	return _BossSprite;
}