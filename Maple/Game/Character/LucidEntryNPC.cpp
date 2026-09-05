#include "pch.h"
#include "LucidEntryNPC.h"
#include "Component/InteractionComponent.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "World/GameLevel.h"
#include "World/MapManager.h"

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

    _Interaction = CreateActorComponent<InteractionComponent>("Interaction");

    if (!_Interaction)
    {
        return false;
    }

    _Interaction->SetInteractionTarget(_Sprite);

    _Interaction->SetInteractionSpace(eInteractionSpace::World);

    _Interaction->SetClickType(eInteractionClickType::DoubleClick);

    _Interaction->SetInteractionCallback(this, &LucidEntryNPC::Interaction);

    return true;
}

void LucidEntryNPC::Interaction()
{
    ChangeToLucidPhase1();
}

void LucidEntryNPC::ChangeToLucidPhase1()
{
    Ptr<GameLevel> CurrentLevel = Cast<Level, GameLevel>(GetLevel());

    if (!CurrentLevel)
    {
        return;
    }

    Ptr<MapManager> Manager = CurrentLevel->GetMapManager();

    if (!Manager)
    {
        return;
    }

    if (Manager->ChangeMap("LucidPhase1"))
    {
        CurrentLevel->StartBossBattle();
    }

    Manager->ChangeMap("LucidPhase1");
}

void LucidEntryNPC::Destroy()
{
    _Interaction = nullptr;

    _Sprite = nullptr;
    
    Actor::Destroy();
}