#include "pch.h"
#include "LucidPhase1.h"
#include "Object/Actor.h"
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
#include "UI/BossHUD.h"
#include "Game/Map/Gate/BossGate.h"
#include "Game/Monsters/Boss/Boss.h"
#include "Game/Monsters/Boss/BossComponent.h"
#include "Game/Monsters/Boss/BossBlackBoard.h"

bool LucidPhase1::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
	if(!Actor::Init(Id, Position, Scale, Rotator, Name))
	{
		return false;
	}

	AddTag("Map.LucidPhase1");

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

	_Floor = CreateSceneComponent<AABBCollisionComponent>("Floor");

	if(!_Floor)
	{
		return false;
	}

	_Floor->SetBoxSize(1792.f, 200.f);
	
	_Floor->SetRelativePosition(213.f, -361.f, 0.f);
	
	_Floor->AttachToComponent(GetRoot());
	
	_Floor->SetCollisionProfile("Environment");


	_LeftWall = CreateSceneComponent<AABBCollisionComponent>("LeftWall");

	if (!_LeftWall)
	{
		return false;
	}

	_LeftWall->SetBoxSize(20.f, 854.f);
	
	_LeftWall->SetRelativePosition(-693.f, -43.f, 0.f);
	
	_LeftWall->AttachToComponent(GetRoot());
	
	_LeftWall->SetCollisionProfile("Environment");


	_RightWall = CreateSceneComponent<AABBCollisionComponent>("RightWall");

	if (!_RightWall)
	{
		return false;
	}

	_RightWall->SetBoxSize(20.f, 854.f);
	
	_RightWall->SetRelativePosition(1119.f, -43.f, 0.f);
	
	_RightWall->AttachToComponent(GetRoot());
	
	_RightWall->SetCollisionProfile("Environment");

	const float LeftBound          = _LeftWall->GetWorldPosition()._x + _LeftWall->GetBoxSize()._x * 0.5f;

	const float RightBound         = _RightWall->GetWorldPosition()._x - _RightWall->GetBoxSize()._x * 0.5f;

	const float CameraZoneWidth    = RightBound - LeftBound;
	
	const float CameraZoneCenterX  = (LeftBound + RightBound) * 0.5f;
	
	const float CameraZoneCenterY  = _LeftWall->GetWorldPosition()._y;
	
	const float CameraZoneHeight   = _LeftWall->GetBoxSize()._y;

	Ptr<CameraZone> AreaCameraZone = GetLevel()->SpawnActor<CameraZone>("LucidPhase1CameraZone", FVector3D(CameraZoneCenterX, CameraZoneCenterY, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!AreaCameraZone)
	{
		return false;
	}

	AreaCameraZone->AddTag("Map.LucidPhase1");

	AreaCameraZone->SetArea(CameraZoneWidth, CameraZoneHeight);

	Ptr<BossGate> Gate = GetLevel()->SpawnActor<BossGate>("LucidPhase1BossGate", FVector3D(-578.f, -261.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!Gate)
	{
		return false;
	}

	Gate->SetTargetMap("LucidEntry");

	Gate->AddTag("Map.LucidPhase1");

	Ptr<Boss> _Boss = GetLevel()->SpawnActor<Boss>("Lucid", FVector3D(213.f, -261.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!_Boss)
	{
		return false;
	}	

	_Boss->AddTag("Map.LucidPhase1");

	if (!InitBossPatternArea(_Boss))
	{
		return false;
	}

	Ptr<BossHUD> BossHUDActor = GetLevel()->SpawnActor<BossHUD>("BossHUD", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!BossHUDActor)
	{
		return false;
	}

	BossHUDActor->AddTag("Map.LucidPhase1");
	
	return true;
}

void LucidPhase1::Tick(float DeltaTime)
{
	Actor::Tick(DeltaTime);
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

bool LucidPhase1::InitBossPatternArea(Ptr<Boss> BossActor)
{
	if (!BossActor || !_LeftWall || !_RightWall || !_Floor)
	{
		return false;
	}

	Ptr<BossComponent> BossController = BossActor->FindActorComponent<BossComponent>("Boss");

	if (!BossController)
	{
		return false;
	}

	Ptr<MonsterBlackBoard> MonsterBoard = BossController->GetBlackBoard();

	if (!MonsterBoard)
	{
		return false;
	}

	Ptr<BossBlackBoard> BossBoard = Cast<MonsterBlackBoard, BossBlackBoard>(MonsterBoard);

	if (!BossBoard)
	{
		return false;
	}

	FBossPatternAreaData& PatternArea = BossBoard->PatternArea;

	PatternArea.LeftBound = _LeftWall->GetWorldPosition()._x + _LeftWall->GetBoxSize()._x * 0.5f;

	PatternArea.RightBound = _RightWall->GetWorldPosition()._x - _RightWall->GetBoxSize()._x * 0.5f;

	PatternArea.GroundY = _Floor->GetWorldPosition()._y + _Floor->GetBoxSize()._y * 0.5f;

	if (PatternArea.SlotCount <= 0)
	{
		return false;
	}

	PatternArea.GroundSpawnPositions.clear();

	PatternArea.GroundSpawnPositions.reserve(PatternArea.SlotCount);

	float ArenaWidth = PatternArea.RightBound - PatternArea.LeftBound;

	float SlotWidth = ArenaWidth / static_cast<float>(PatternArea.SlotCount);

	for (int32 Index = 0; Index < PatternArea.SlotCount; ++Index)
	{
		float SpawnX = PatternArea.LeftBound + SlotWidth * static_cast<float>(Index) + SlotWidth * 0.5f;

		PatternArea.GroundSpawnPositions.push_back(FVector3D(SpawnX, PatternArea.GroundY, 0.f));
	}

	return true;
}
