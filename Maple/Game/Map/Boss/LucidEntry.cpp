#include "pch.h"
#include "LucidEntry.h"
#include "World/World.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "Core/Animation2DData.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Render/RenderManager.h"
#include "Object/Actor.h"

LucidEntry::LucidEntry()
{
}

LucidEntry::~LucidEntry()
{
}

bool LucidEntry::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    if (!ANIMATION_MANAGER->LoadAnimationFile(TEXT("Maps\\Boss\\Entry\\LucidEntry.json"), "Animations"))
    {
        return false;
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
