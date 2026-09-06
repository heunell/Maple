#include "pch.h"
#include "Boss.h"
#include "BossComponent.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/AABBCollisionComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "World/GameLevel.h"
#include "World/MapManager.h"

bool Boss::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!MonsterBase::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("Monster");

	AddTag("Boss");

	_Status.MaxHP = 1000000000;

	_Status.CurrentHP = _Status.MaxHP;

	if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Monsters\\Lucid\\Phase1\\LucidPhase1.json"), "Animations"))
	{
		return false;
	}

	Ptr<Animation2DData> BottomStandData = ANIMATION_MANAGER->FindAnimation("LUCID_BOSS_8880158.stand");

	if (!BottomStandData)
	{
		return false;
	}

	Ptr<Animation2DData> BossStandData = ANIMATION_MANAGER->FindAnimation("LUCID_MOB_8880140.stand");

	if (!BossStandData)
	{
		return false;
	}


	// 158 리소스는 루시드의 아래쪽 몸체로 뒤에 출력한다.
	_BottomSprite = CreateSceneComponent<SpriteComponent>("BossBottomSprite");

	if (!_BottomSprite)
	{
		return false;
	}

	_BottomSprite->SetRenderLayerName("Default");

	_BottomSprite->AddAnimationSequence(BottomStandData, true);

	_BottomSprite->AddAnimationSequence("LUCID_BOSS_8880158.die", false);

	_BottomSprite->AttachToComponent(GetRoot());

	_BottomSprite->SetRelativePosition(0.f, 0.f, 0.f);


	// 140 리소스는 루시드의 위쪽 몸체로 패턴 애니메이션을 담당한다.
	_BossSprite = CreateSceneComponent<SpriteComponent>("BossSprite");

	if (!_BossSprite)
	{
		return false;
	}

	_BossSprite->SetRenderLayerName("Default");

	_BossSprite->AddAnimationSequence(BossStandData, true);

	_BossSprite->AddAnimationSequence("LUCID_MOB_8880140.die1"  , false);   // DIE

	_BossSprite->AddAnimationSequence("LUCID_MOB_8880140.skill1", false);	// Blade
	
	_BossSprite->AddAnimationSequence("LUCID_MOB_8880140.skill2", false);	// Dragon
	
	_BossSprite->AddAnimationSequence("LUCID_MOB_8880140.skill3", false);	// Mushroom & Golem

	_BossSprite->AddAnimationSequence("LUCID_MOB_8880140.skill4", false);   // Teleport

	_BossSprite->AttachToComponent(GetRoot());

	_BossSprite->SetRelativePosition(0.f, 0.f, 0.1f);

	Ptr<AABBCollisionComponent> TopCollision = CreateSceneComponent<AABBCollisionComponent>("BossTopCollision");

	if (!TopCollision)
	{
		return false;
	}

	TopCollision->SetBoxSize(210.f, 120.f);

	TopCollision->SetRelativePosition(20.f, 290.f, 0.f);
	
	TopCollision->AttachToComponent(GetRoot());
	
	TopCollision->SetCollisionProfile("Boss");

	Ptr<AABBCollisionComponent> BottomCollision = CreateSceneComponent<AABBCollisionComponent>("BossBottomCollision");

	if (!BottomCollision)
	{
		return false;
	}

	BottomCollision->SetBoxSize(95.f, 230.f);

	BottomCollision->SetRelativePosition(-25.f, 115.f, 0.f);
	
	BottomCollision->AttachToComponent(GetRoot());
	
	BottomCollision->SetCollisionProfile("Boss");

	Ptr<BossComponent> Component = CreateActorComponent<BossComponent>("Boss");

	if (!Component)
	{
		return false;
	}

	return true;
}

void Boss::Tick(float DeltaTime)
{
	if (_Status.CurrentHP > 0)
	{
		MonsterBase::Tick(DeltaTime);
		return;
	}

	FindSceneComponent<AABBCollisionComponent>("BossTopCollision")->SetEnable(false);

	FindSceneComponent<AABBCollisionComponent>("BossBottomCollision")->SetEnable(false);

	_BossSprite->ChangeAnimation("LUCID_MOB_8880140.die1");

	_BottomSprite->ChangeAnimation("LUCID_BOSS_8880158.die");

	GetRoot()->Tick(DeltaTime);

	if (!_BossSprite->GetAnimation()->IsFinished() || !_BottomSprite->GetAnimation()->IsFinished())
	{
		return;
	}

	if (Ptr<GameLevel> CurrentLevel = Cast<Level, GameLevel>(GetLevel()))
	{
		if (CurrentLevel->GetMapManager())
		{
			CurrentLevel->GetMapManager()->ChangeMap("LucidPhase2");
		}
	}
}

Ptr<class SpriteComponent> Boss::GetBossSprite() const
{
	return _BossSprite;
}

void Boss::ResetBattle()
{
	_Status.CurrentHP = _Status.MaxHP;

	FindSceneComponent<AABBCollisionComponent>("BossTopCollision")->SetEnable(true);

	FindSceneComponent<AABBCollisionComponent>("BossBottomCollision")->SetEnable(true);

	Ptr<BossComponent> Component = FindActorComponent<BossComponent>("Boss");

	if (Component)
	{
		Component->ResetBattle();
	}

	_BossSprite->ChangeAnimation("LUCID_MOB_8880140.stand");

	if (_BossSprite)
	{
		_BossSprite->SetAnimationFrame(0);

		_BossSprite->SetPlay("LUCID_MOB_8880140.stand", true);
	}

	_BottomSprite->ChangeAnimation("LUCID_BOSS_8880158.stand");

	if (_BottomSprite)
	{
		_BottomSprite->SetAnimationFrame(0);

		_BottomSprite->SetPlay("LUCID_BOSS_8880158.stand", true);
	}
}
