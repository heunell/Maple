#include "pch.h"
#include "Boss2.h"
#include "Boss2Component.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"

bool Boss2::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if (!MonsterBase::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("Monster");

	AddTag("Boss");

	if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Monsters\\Lucid\\Phase2\\LucidPhase2.json"), "Animations"))
	{
		return false;
	}

	// 공통 리소스를 사용하기 위함
	if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Monsters\\Lucid\\Phase1\\LucidPhase1.json"), "Animations"))
	{
		return false;
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

	_BossSprite->AddAnimationSequence("LUCID_MOB_8880150.skill1", false);

	_BossSprite->AttachToComponent(GetRoot());

	Ptr<Boss2Component> Component = CreateActorComponent<Boss2Component>("Boss2");

	if (!Component)
	{
		return false;
	}

	return true;
}

Ptr<SpriteComponent> Boss2::GetBossSprite() const
{
	return _BossSprite;
}