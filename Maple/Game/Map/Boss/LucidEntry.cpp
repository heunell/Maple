#include "pch.h"
#include "LucidEntry.h"
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
#include "Game/Character/LucidEntryNPC.h"

LucidEntry::LucidEntry()
{
}

LucidEntry::~LucidEntry()
{
}

bool LucidEntry::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    return Init(Id, Position, Scale, Rotator, Name, eRoomType::Entry);
}

bool LucidEntry::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, eRoomType RoomType)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    const std::string MapTag = RoomType == eRoomType::Entry ? "Map.LucidEntry" : "Map.LucidReward";

    AddTag(MapTag);   // MapManager가 LucidEntry 소속 Actor를 하나의 그룹으로 제어하고 같은 배경을 사용하는 reward 맵을 논리맵으로 두고 tag에 따라서 Actor의 배치를 결정한다.

    // Entry와 Reward가 같은 배경 애니메이션을 중복 등록하지 않게 한다.
    if (!ANIMATION_MANAGER->FindAnimation("LUCID_ENTRY_SKY_TILE_-08"))
    {
        if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Maps\\Boss\\Entry\\LucidEntry.json"), "Animations"))
        {
            return false;
        }
    }

    // NPC 애니메이션은 Entry 논리 맵에서만 필요하다.
    if (RoomType == eRoomType::Entry && !ANIMATION_MANAGER->FindAnimation("LUCID_ENTRY_NPC"))
    {
        if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Maps\\Boss\\Entry\\LucidEntryNpc.json"), "Animations"))
        {
            return false;
        }
    }

    std::vector<std::string> StaticParts =
    {

        "LUCID_ENTRY_SKY_TILE_-08",
        "LUCID_ENTRY_SKY_TILE_-07",
        "LUCID_ENTRY_SKY_TILE_-06",
        "LUCID_ENTRY_SKY_TILE_-05",
        "LUCID_ENTRY_SKY_TILE_-04",
        "LUCID_ENTRY_SKY_TILE_-03",
        "LUCID_ENTRY_SKY_TILE_-02",
        "LUCID_ENTRY_SKY_TILE_-01",
        "LUCID_ENTRY_SKY_TILE_+00",
        "LUCID_ENTRY_SKY_TILE_+01",
        "LUCID_ENTRY_SKY_TILE_+02",
        "LUCID_ENTRY_SKY_TILE_+03",
        "LUCID_ENTRY_SKY_TILE_+04",
        "LUCID_ENTRY_SKY_TILE_+05",
        "LUCID_ENTRY_SKY_TILE_+06",
        "LUCID_ENTRY_SKY_TILE_+07",
        "LUCID_ENTRY_SKY_TILE_+08",
        "LUCID_ENTRY_SKY_TILE_+09",
        "LUCID_ENTRY_SKY_TILE_+10",
        "LUCID_ENTRY_SKY_TILE_+11",
        "LUCID_ENTRY_SKY_TILE_+12",

        "LUCID_ENTRY_OBJ_20",
        "LUCID_ENTRY_OBJ_22",
        "LUCID_ENTRY_OBJ_23",
        "LUCID_ENTRY_OBJ_21",
        "LUCID_ENTRY_OBJ_24",
        "LUCID_ENTRY_OBJ_25_LEFT",
        "LUCID_ENTRY_OBJ_25_RIGHT",

        "LUCID_ENTRY_BACK_42"

    };

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

    if(RoomType == eRoomType::Entry)
    {
        Ptr<LucidEntryNPC> NPC = GetLevel()->SpawnActor<LucidEntryNPC>("LucidEntryNPC", FVector3D(191.f, -158.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

        if (!NPC)
        {
            return false;
        }

        NPC->AddTag(MapTag);
    }

    // 지면 충돌 콜리전
    _Floor = CreateSceneComponent<AABBCollisionComponent>("Floor");

    if (!_Floor)
    {
        return false;
    }

    _Floor->SetBoxSize(770.f, 200.f);
    _Floor->SetRelativePosition(155.f, -348.f, 0.f);
    _Floor->AttachToComponent(GetRoot());
    _Floor->SetCollisionProfile("Environment");


    _LeftWall = CreateSceneComponent<AABBCollisionComponent>("LeftWall");

    if (!_LeftWall)
    {
        return false;
    }

    _LeftWall->SetBoxSize(20.f, 768.f);
    _LeftWall->SetRelativePosition(-240.f, 0.f, 0.f);
    _LeftWall->AttachToComponent(GetRoot());
    _LeftWall->SetCollisionProfile("Environment");


    _RightWall = CreateSceneComponent<AABBCollisionComponent>("RightWall");

    if (!_RightWall)
    {
        return false;
    }

    _RightWall->SetBoxSize(20.f, 768.f);
    _RightWall->SetRelativePosition(550.f, 0.f, 0.f);
    _RightWall->AttachToComponent(GetRoot());
    _RightWall->SetCollisionProfile("Environment");

    const float LeftBound = _LeftWall->GetWorldPosition()._x + _LeftWall->GetBoxSize()._x * 0.5f;

    const float RightBound = _RightWall->GetWorldPosition()._x - _RightWall->GetBoxSize()._x * 0.5f;

    const float CameraZoneWidth = RightBound - LeftBound;

    const float CameraZoneCenterX = (LeftBound + RightBound) * 0.5f;

    const float CameraZoneCenterY = _LeftWall->GetWorldPosition()._y;

    const float CameraZoneHeight = _LeftWall->GetBoxSize()._y;

    Ptr<CameraZone> AreaCameraZone = GetLevel()->SpawnActor<CameraZone>(Name + "CameraZone", FVector3D(CameraZoneCenterX, CameraZoneCenterY, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f));

    if (!AreaCameraZone)
    {
        return false;
    }

    AreaCameraZone->AddTag(MapTag);

    AreaCameraZone->SetArea(CameraZoneWidth, CameraZoneHeight);

    return true;
}

void LucidEntry::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);
}

void LucidEntry::Collision(float DeltaTime)
{
    Actor::Collision(DeltaTime);

}

void LucidEntry::Render(float DeltaTime)
{
}

void LucidEntry::Destroy()
{
    Actor::Destroy();
}
