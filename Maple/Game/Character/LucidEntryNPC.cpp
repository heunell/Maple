#include "pch.h"
#include "LucidEntryNPC.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"

bool LucidEntryNPC::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation("LUCID_ENTRY_NPC");

    if (!Data)
    {
        return false;
    }

    _Sprite = CreateSceneComponent<SpriteComponent>("NPCSprite");

    if(!_Sprite)
    {
        return false;
    }

    _Sprite->SetRenderLayerName("Default");
    
    _Sprite->AddAnimationSequence(Data);

    _Sprite->AttachToComponent(GetRoot());

    return true;
}

void LucidEntryNPC::Destroy()
{
    _Sprite = nullptr;

    Actor::Destroy();
}
