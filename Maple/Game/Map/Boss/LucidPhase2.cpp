#include "pch.h"
#include "LucidPhase2.h"

#include "World/World.h"
#include "World/Level.h"
#include "World/CameraZone.h"

#include "Core/AnimationManager.h"
#include "Core/Animation2DData.h"
#include "Core/AssetManager.h"
#include "Core/GameEngine.h"

#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/ParallaxComponent.h"
#include "Component/CameraComponent.h"
#include "Component/MovementComponent.h"

#include "Game/Monsters/Boss/Boss2.h"
#include "UI/BossHUD.h"

#include <random>

bool LucidPhase2::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    AddTag("Map.LucidPhase2");

    if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Maps\\Boss\\Phase2\\LucidPhase2.json"), "Animations"))
    {
        return false;
    }

    Ptr<SceneComponent> ScreenBackgroundRoot = CreateSceneComponent<SceneComponent>("ScreenBackgroundRoot");

    Ptr<SceneComponent> FarBackgroundRoot = CreateSceneComponent<SceneComponent>("FarBackgroundRoot");

    Ptr<SceneComponent> MiddleBackgroundRoot = CreateSceneComponent<SceneComponent>("MiddleBackgroundRoot");

    Ptr<SceneComponent> NearBackgroundRoot = CreateSceneComponent<SceneComponent>("NearBackgroundRoot");

    Ptr<SceneComponent> SkyRoot = CreateSceneComponent<SceneComponent>("SkyRoot");

    if (!ScreenBackgroundRoot || !FarBackgroundRoot || !MiddleBackgroundRoot || !NearBackgroundRoot || !SkyRoot)
    {
        return false;
    }

    ScreenBackgroundRoot->SetRelativeScale(1.f, 1.f, 1.f);
    
    FarBackgroundRoot->SetRelativeScale(1.f, 1.f, 1.f);
    
    MiddleBackgroundRoot->SetRelativeScale(1.f, 1.f, 1.f);
    
    NearBackgroundRoot->SetRelativeScale(1.f, 1.f, 1.f);

    SkyRoot->SetRelativeScale(1.f, 1.5f, 1.f);

    ScreenBackgroundRoot->AttachToComponent(GetRoot());

    FarBackgroundRoot->AttachToComponent(GetRoot());

    MiddleBackgroundRoot->AttachToComponent(GetRoot());

    NearBackgroundRoot->AttachToComponent(GetRoot());

    SkyRoot->AttachToComponent(ScreenBackgroundRoot);

    _BackgroundScrollLayers.reserve(3);

    _BackgroundScrollLayers.push_back({ FarBackgroundRoot, 10.f });
    
    _BackgroundScrollLayers.push_back({ MiddleBackgroundRoot, 120.f });
    
    _BackgroundScrollLayers.push_back({ NearBackgroundRoot, 170.f });

    std::vector<FBackgroundPart> BackgroundParts;

    BackgroundParts.reserve(116);

    const std::vector<std::string> XSuffixes =
    {
        "-04", "-03", "-02", "-01",
        "+00", "+01", "+02", "+03", "+04",
        "+05", "+06", "+07", "+08", "+09",
        "+10", "+11", "+12", "+13", "+14"
    };

    const std::vector<std::string> YSuffixes =
    {
        "+01"
    };

    // 화면을 채우는 반복 배경
    for (const std::string& YSuffix : YSuffixes)
    {
        for (const std::string& XSuffix : XSuffixes)
        {
            const std::string AnimationName = "LUCID_PHASE2_BACK_9_TILE_X" + XSuffix + "_Y" + YSuffix;

            BackgroundParts.push_back({ AnimationName, SkyRoot });
        }
    }

    // 카메라에 고정되는 밤하늘과 별
    BackgroundParts.push_back({ "LUCID_PHASE2_ENV_BACK_72", ScreenBackgroundRoot });

    BackgroundParts.push_back({ "LUCID_PHASE2_BACK_3", ScreenBackgroundRoot });

    BackgroundParts.push_back({ "LUCID_PHASE2_BACK_4", ScreenBackgroundRoot });

    BackgroundParts.push_back({ "LUCID_PHASE2_BACK_5", ScreenBackgroundRoot });

    // 상승하는 먼 환경 파츠
    BackgroundParts.push_back({ "LUCID_PHASE2_ENV_BACK_80", FarBackgroundRoot });

    // 큰 건물 잔해
    const std::vector<std::string> FarBackgroundNames =
    {
        "LUCID_PHASE2_BACK_17",
        "LUCID_PHASE2_BACK_16",
        "LUCID_PHASE2_BACK_15",
        "LUCID_PHASE2_BACK_14",
        "LUCID_PHASE2_BACK_13",
        "LUCID_PHASE2_BACK_12"
    };

    for (const std::string& PartName : FarBackgroundNames)
    {
        BackgroundParts.push_back({ PartName, FarBackgroundRoot });
    }

    // 작은 건물 잔해
    const std::vector<std::string> MiddleBackgroundNames =
    {
        "LUCID_PHASE2_BACK_68",
        "LUCID_PHASE2_BACK_67",
        "LUCID_PHASE2_BACK_66",
        "LUCID_PHASE2_BACK_65",
        "LUCID_PHASE2_BACK_64",
        "LUCID_PHASE2_BACK_63"
    };

    for (const std::string& PartName : MiddleBackgroundNames)
    {
        BackgroundParts.push_back({ PartName, MiddleBackgroundRoot });
    }

    // 카메라에 가장 가까운 잔해
    const std::vector<std::string> NearBackgroundNames =
    {
        "LUCID_PHASE2_BACK_82",
        "LUCID_PHASE2_BACK_83",
        "LUCID_PHASE2_BACK_84",
        "LUCID_PHASE2_BACK_85"
    };

    for (const std::string& PartName : NearBackgroundNames)
    {
        BackgroundParts.push_back({ PartName, NearBackgroundRoot });
    }

    for (const FBackgroundPart& Part : BackgroundParts)
    {
        Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(Part.Name);

        if (!Data)
        {
            return false;
        }

        Ptr<SpriteComponent> Sprite = CreateSceneComponent<SpriteComponent>(Part.Name);

        if (!Sprite)
        {
            return false;
        }

        Sprite->SetRenderLayerName("BackGround");

        Sprite->AddAnimationSequence(Data);

        Sprite->ChangeAnimation(Part.Name);

        Sprite->AttachToComponent(Part.Parent);
    }

    Ptr<ParallaxComponent> BackgroundParallax = CreateActorComponent<ParallaxComponent>("BackgroundParallax");

    if(!BackgroundParallax)
    {
        return false;
    }

    Ptr<CameraComponent> ParallaxCamera = GetLevel()->GetMainCamera();

    if(!ParallaxCamera)
    {
        return false;
    }

    ScreenBackgroundRoot->SetRelativePosition(0.f, ParallaxCamera->GetRelativePosition()._y, 0.f);
    
    BackgroundParallax->SetCamera(ParallaxCamera);

    if(!BackgroundParallax->AddLayer(ScreenBackgroundRoot, 1.f))
    {
        return false;
    }

    const std::vector<std::string> PlatformNames =
    {
        "BBLUE3",
        "BRED1",
        "BBLUE2",
        "BRED3",
        "MYELLOW3",
        "MYELLOW1",
        "EXCEPT1",
        "MYELLOW2",
        "BRED2",
        "EXCEPT4",
        "EXCEPT2",
        "BBLUE1",
        "STATIC_LEFT",
        "STATIC_MID",
        "STATIC_RIGHT"
    };

    for (const std::string& PlatformName : PlatformNames)
    {
        const std::string IdleName = "LUCID_PHASE2_PLATFORM_" + PlatformName + "_IDLE";

        const std::string BreakName = "LUCID_PHASE2_PLATFORM_" + PlatformName + "_BREAK";

        Ptr<Animation2DData> IdleData = ANIMATION_MANAGER->FindAnimation(IdleName);

        Ptr<Animation2DData> BreakData = ANIMATION_MANAGER->FindAnimation(BreakName);

        if (!IdleData || !BreakData)
        {
            return false;
        }

        Ptr<SpriteComponent> PlatformSprite = CreateSceneComponent<SpriteComponent>(IdleName);

        if (!PlatformSprite)
        {
            return false;
        }

        PlatformSprite->SetRenderLayerName("Default");

        PlatformSprite->AddAnimationSequence(IdleData, true);

        PlatformSprite->AddAnimationSequence(BreakData, false);

        PlatformSprite->ChangeAnimation(IdleName);

        PlatformSprite->AttachToComponent(GetRoot());
    }

    const FPlatformCollisionData PlatformCollisions[] =
    {
        {
            "Bblue3Collision",      FVector3D(306.f, -1228.f, 0.f),  FVector2D(350.f, 18.f)
        },                                                           
        {                                                            
            "Bred1Collision",       FVector3D(1100.f, -734.f, 0.f),  FVector2D(350.f, 18.f)
        },                                                           
        {                                                            
            "Bblue2Collision",      FVector3D(711.f, -863.f, 0.f),   FVector2D(350.f, 18.f)
        },                                                           
        {                                                            
            "Bred3Collision",       FVector3D(792.f, -1159.f, 0.f),  FVector2D(350.f, 18.f)
        },                                                           
        {                                                            
            "Myellow3Collision",    FVector3D(1150.f, -1210.f, 0.f), FVector2D(260.f, 18.f)
        },                          
        {                           
            "Myellow1Collision",    FVector3D(530.f, -668.f, 0.f),   FVector2D(260.f, 18.f)
        },                          
        {                           
            "Except1Collision",     FVector3D(1012.f, -514.f, 0.f),  FVector2D(180.f, 18.f)
        },                           
        {                           
            "Myellow2Collision",    FVector3D(124.f, -1086.f, 0.f),  FVector2D(260.f, 18.f)
        },                          
        {                           
            "Bred2Collision",       FVector3D(157.f, -803.f, 0.f),   FVector2D(350.f, 18.f)
        },                          
        {                           
            "Except4Collision",     FVector3D(603.f, -1308.f, 0.f),  FVector2D(180.f, 18.f)
        },                          
        {                            
            "Except2Collision",     FVector3D(71.f, -662.f, 0.f),    FVector2D(170.f, 18.f)
        },                          
        {                           
            "Bblue1Collision",      FVector3D(323.f, -498.f, 0.f),   FVector2D(350.f, 18.f)
        },
        {
            "StaticLeftCollision",  FVector3D(375.f, -976.f, 0.f),   FVector2D(260.f, 18.f)
        },                          
        {                           
            "StaticMidCollision",   FVector3D(911.f, -976.f, 0.f),   FVector2D(260.f, 18.f)
        },
        {
            "StaticRightCollision", FVector3D(1205.f, -976.f, 0.f),  FVector2D(220.f, 18.f)
        }
    };

    for (const FPlatformCollisionData& Platform : PlatformCollisions)
    {
        Ptr<AABBCollisionComponent> Collision = CreateSceneComponent<AABBCollisionComponent>(Platform.Name);

        if (!Collision)
        {
            return false;
        }

        Collision->SetBoxSize(Platform.Size._x, Platform.Size._y);

        Collision->SetRelativePosition(Platform.Position);

        Collision->AttachToComponent(GetRoot());

        Collision->SetCollisionProfile("Environment");

        FVector3D RegenPosition = Platform.Position;

        RegenPosition._y += Platform.Size._y * 0.5f;
        
        _PlatformRegenPositions.push_back(RegenPosition);
    }

    _LeftWall = CreateSceneComponent<AABBCollisionComponent>("LeftWall");

    if (!_LeftWall)
    {
        return false;
    }

    _LeftWall->SetBoxSize(20.f, 2400.f);

    _LeftWall->SetRelativePosition(-693.f, -816.f, 0.f);

    _LeftWall->AttachToComponent(GetRoot());

    _LeftWall->SetCollisionProfile("Environment");

    _RightWall = CreateSceneComponent<AABBCollisionComponent>("RightWall");

    if (!_RightWall)
    {
        return false;
    }

    _RightWall->SetBoxSize(20.f, 2400.f);

    _RightWall->SetRelativePosition(1827.f, -816.f, 0.f);

    _RightWall->AttachToComponent(GetRoot());

    _RightWall->SetCollisionProfile("Environment");

    float LeftBound = _LeftWall->GetWorldPosition()._x + _LeftWall->GetBoxSize()._x * 0.5f;

    float RightBound = _RightWall->GetWorldPosition()._x - _RightWall->GetBoxSize()._x * 0.5f;
    
    float CameraHalfHeight = ParallaxCamera->GetHeight() * 0.5f;
    
    float CameraZoneBottom = _CameraMinCenterY - CameraHalfHeight;
    
    float CameraZoneTop = _CameraMaxCenterY + CameraHalfHeight;
    
    float CameraZoneWidth = RightBound - LeftBound;
    
    float CameraZoneHeight = CameraZoneTop - CameraZoneBottom;
    
    float CameraZoneCenterX = (LeftBound + RightBound) * 0.5f;
    
    float CameraZoneCenterY = (CameraZoneBottom + CameraZoneTop) * 0.5f;

    Ptr<CameraZone> AreaCameraZone = GetLevel()->SpawnActor<CameraZone>("LucidPhase2CameraZone", FVector3D(CameraZoneCenterX, CameraZoneCenterY, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

    if (!AreaCameraZone)
    {
        return false;
    }

    AreaCameraZone->AddTag("Map.LucidPhase2");

    AreaCameraZone->SetArea(CameraZoneWidth, CameraZoneHeight);

    Ptr<Boss2> BossMonster = GetLevel()->SpawnActor<Boss2>("LucidPhase2Boss", FVector3D(911.f, -700.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

	if (!BossMonster)
	{
		return false;
	}

    BossMonster->AddTag("Map.LucidPhase2");

    Ptr<BossHUD> BossHUDActor = GetLevel()->SpawnActor<BossHUD>("BossHUD", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

    if (!BossHUDActor)
    {
        return false;
    }

    return true;
}

void LucidPhase2::RegenPlayerPlatform(Ptr<Actor> Player)
{
	if (!Player || _PlatformRegenPositions.empty())
	{
		return;
	}

	std::random_device RandomDevice;

	std::mt19937 RandomEngine(RandomDevice());

	std::uniform_int_distribution<int32> PlatformDistribution(0, static_cast<int32>(_PlatformRegenPositions.size()) - 1);

	int32 PlatformIndex = PlatformDistribution(RandomEngine);

	FVector3D Destination = _PlatformRegenPositions[PlatformIndex];

	Destination._z = Player->GetWorldPosition()._z;

	Player->SetWorldPosition(Destination);

	Ptr<MovementComponent> Movement = Player->FindActorComponent<MovementComponent>("Movement");

	if (Movement)
	{
		Movement->ResetMovement();
	}
}

void LucidPhase2::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);

    for (const FBackgroundScrollLayer& Layer : _BackgroundScrollLayers)
    {
        Ptr<SceneComponent> BackgroundRoot = Lock(Layer.Root);

        if (!BackgroundRoot)
        {
            continue;
        }

        FVector3D Position = BackgroundRoot->GetRelativePosition();

        Position._y += Layer.ScrollSpeed * DeltaTime;

        BackgroundRoot->SetRelativePosition(Position);
    }

    Ptr<World> CurrentWorld = GameEngine::Instance().GetWorld();

    if (!CurrentWorld)
    {
        return;
    }

    Ptr<Actor> Player = CurrentWorld->GetPlayer();

    Ptr<CameraComponent> Camera = GetLevel()->GetMainCamera();

    if (!Player || !Camera)
    {
        return;
    }

    float CameraBottom = Camera->GetWorldPosition()._y - Camera->GetHeight() * 0.5f;

    if (Player->GetWorldPosition()._y < CameraBottom - 600.f)
    {
        RegenPlayerPlatform(Player);
    }
}