#include "pch.h"
#include "LucidEntryNPC.h"
#include "Component/InteractionComponent.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "World/GameLevel.h"
#include "World/MapManager.h"
#include "UI/LucidEntryDialogUI.h"

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
    if (!_DialogUI)
    {
        Ptr<Level> CurrentLevel = GetLevel();
        
        if (!CurrentLevel)
        {
            return;
        }
        
        _DialogUI = CurrentLevel->SpawnActor<LucidEntryDialogUI>("LucidEntryDialogUI", FVector3D(0.f, 0.f, 0.f), FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), This<LucidEntryNPC>());
        
        if (!_DialogUI)
        {
            return;
        }
    }
    
    _DialogUI->Open();
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

    //Manager->ChangeMap("LucidPhase1"); // 중복호출로 인한 제거 Confirm함수에서 호출하고 있다.
}

void LucidEntryNPC::Destroy()
{   
    if (_DialogUI)
    {
        _DialogUI->Close();
        
        _DialogUI->Remove();
        
        _DialogUI = nullptr;
    }
    _Interaction = nullptr;

    _Sprite = nullptr;
    
    Actor::Destroy();
}