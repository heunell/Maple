#include "pch.h"
#include "Boss.h"
#include "BossComponent.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"


bool Boss::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!MonsterBase::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("Monster");

	AddTag("Boss");

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

	_BossSprite->AddAnimationSequence("LUCID_MOB_8880140.skill1", false);	// Blade
	
	_BossSprite->AddAnimationSequence("LUCID_MOB_8880140.skill3", false);	// Mushroom

	_BossSprite->AttachToComponent(GetRoot());

	_BossSprite->SetRelativePosition(0.f, 0.f, 0.1f);


	Ptr<BossComponent> Component = CreateActorComponent<BossComponent>("Boss");

	if (!Component)
	{
		return false;
	}

	return true;
}

Ptr<class SpriteComponent> Boss::GetBossSprite() const
{
	return _BossSprite;
}
