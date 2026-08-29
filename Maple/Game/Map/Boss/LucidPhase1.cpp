#include "pch.h"
#include "LucidPhase1.h"
#include "World/World.h"
#include "World/Level.h"
#include "World/CameraZone.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "Core/Animation2DData.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Render/RenderManager.h"
#include "Object/Actor.h"

bool LucidPhase1::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if(!Actor::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Maps\\Boss\\Phase1\\Lucid_Phase1_Level.json"), "Animations"))
    {
        return false;
    }

	const std::vector<std::string> TileSuffixes = 
	{
		"-05", "-04", "-03", "-02", "-01",
		"+00", "+01", "+02", "+03", "+04",
		"+05", "+06", "+07", "+08"
	};

	const std::vector<std::string> BackTilePrefixes =
	{
		"LUCID_PHASE1_BACK_69_TILE_",
		"LUCID_PHASE1_BACK_79_TILE_",
		"LUCID_PHASE1_BACK_78_TILE_",
		"LUCID_PHASE1_BACK_77_TILE_"
	};

	std::vector<std::string> StaticParts;

	StaticParts.reserve(85);

	for (const std::string& Prefix : BackTilePrefixes)
	{
		for (const std::string& Suffix : TileSuffixes)
		{
			StaticParts.emplace_back(Prefix + Suffix);
		}
	}

	StaticParts.insert(StaticParts.end(),
	{
		"LUCID_PHASE1_BACK_73_76_TILE_00_BACK_73",
		"LUCID_PHASE1_BACK_73_76_TILE_01_BACK_74",
		"LUCID_PHASE1_BACK_73_76_TILE_02_BACK_75",
		"LUCID_PHASE1_BACK_73_76_TILE_03_BACK_76",
		"LUCID_PHASE1_BACK_73_76_TILE_04_BACK_73",
		"LUCID_PHASE1_BACK_73_76_TILE_05_BACK_74",
		"LUCID_PHASE1_BACK_73_76_TILE_06_BACK_75",
		"LUCID_PHASE1_BACK_73_76_TILE_07_BACK_76",
		"LUCID_PHASE1_BACK_73_76_TILE_08_BACK_73",
		"LUCID_PHASE1_BACK_73_76_TILE_09_BACK_74",
		"LUCID_PHASE1_BACK_73_76_TILE_10_BACK_75",
		"LUCID_PHASE1_BACK_73_76_TILE_11_BACK_76",
		"LUCID_PHASE1_BACK_73_76_TILE_12_BACK_73",
		"LUCID_PHASE1_BACK_73_76_TILE_13_BACK_74"
	});

	for (const std::string& Suffix : TileSuffixes)
	{
		StaticParts.emplace_back("LUCID_PHASE1_FOOTHOLD_6_TILE_" + Suffix);
	}

	StaticParts.emplace_back("LUCID_PHASE1_BACK_81");

	for (const std::string& PartName : StaticParts)
	{
		Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(PartName);

		if (!Data)
		{
			return false;
		}

		Ptr<SpriteComponent> Sprite = CreateSceneComponent<SpriteComponent>(PartName);

		if (!Sprite)
		{
			return false;
		}

		Sprite->SetRenderLayerName("BackGround");

		Sprite->AddAnimationSequence(Data);
		
		Sprite->AttachToComponent(GetRoot());
	}

	return true;
}

void LucidPhase1::Tick(float DeltaTime)
{
}

void LucidPhase1::Collision(float DeltaTime)
{
}

void LucidPhase1::Render(float DeltaTime)
{
}

void LucidPhase1::Destroy()
{
}
