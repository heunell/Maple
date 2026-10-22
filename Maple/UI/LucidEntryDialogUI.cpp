#include "pch.h"
#include "LucidEntryDialogUI.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/InteractionComponent.h"
#include "Game/Character/LucidEntryNPC.h"
#include "Input/InputSystem.h"

bool LucidEntryDialogUI::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<class LucidEntryNPC> NPC)
{
    if (!UI::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }
    
    if (!NPC)
    {
        return false;
    }
    
    _NPC = NPC;
    
    if (!CreateSprite("EntryBackground", "BOSS_ENTER.main.backgrnd"))
    {
        return false;
    }
    
    if (!CreateSprite("LucidImage", "BOSS_ENTER.bossList.19.mob"))
    {
        return false;
    }

    Ptr<SpriteComponent> Sprite = CreateSprite("EntryButton", "BOSS_ENTER.main.realMode_wnd.button_entry.normal.0");
    
    if (!Sprite)
    {
        return false;
    }
    
    Sprite->AddAnimationSequence("BOSS_ENTER.main.realMode_wnd.button_entry.mouseOver.0");

    Sprite->AddAnimationSequence("BOSS_ENTER.main.realMode_wnd.button_entry.pressed.0");
    
    Ptr<InteractionComponent> Interaction = CreateActorComponent<InteractionComponent>("EntryInteraction");
    
    if (!Interaction)
    {
        return false;
    }
    
    Interaction->SetInteractionTarget(Sprite);
    Interaction->SetInteractionSpace(eInteractionSpace::Screen);
    Interaction->SetClickType(eInteractionClickType::Click);
    Interaction->SetInteractionCallback(this, &LucidEntryDialogUI::Confirm);
    
    Sprite = CreateSprite("CloseButton", "BOSS_ENTER.main.button_close.normal.0");
    
    if (!Sprite)
    {
        return false;
    }
    
    Sprite->AddAnimationSequence("BOSS_ENTER.main.button_close.mouseOver.0");

    Sprite->AddAnimationSequence("BOSS_ENTER.main.button_close.pressed.0");
    
    Interaction = CreateActorComponent<InteractionComponent>("CloseInteraction");

    if (!Interaction)
    {
        return false;
    }

    Interaction->SetInteractionTarget(Sprite);
    Interaction->SetInteractionSpace(eInteractionSpace::Screen);
    Interaction->SetClickType(eInteractionClickType::Click);
    Interaction->SetInteractionCallback(this, &LucidEntryDialogUI::Close);
    
    SetEnable(false);
    
    return true;
}

void LucidEntryDialogUI::Tick(float DeltaTime)
{
    Ptr<LucidEntryNPC> NPC = Lock<LucidEntryNPC>(_NPC);
    
    if (!NPC || !NPC->IsActive() || !NPC->IsEnable())
    {
        Close();
        
        return;
    }
    
    if (_WaitRelease)
    {
        GetRoot()->Tick(DeltaTime);
        
        if (!InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton) && !InputSystem::Instance().GetMouseHold(MOUSE_BUTTON_TYPE::LButton))
        {
            _WaitRelease = false;
        }
    }
    else
    {
        UI::Tick(DeltaTime);
    }
        
    if (!IsEnable())
    {
        return;
    }
    
    Ptr<SpriteComponent> Sprite = FindSceneComponent<SpriteComponent>("EntryButton");
    
    Ptr<InteractionComponent> Interaction = FindActorComponent<InteractionComponent>("EntryInteraction");
    
    if (Sprite && Interaction)
    {
        if (Interaction->IsMouseInsideTarget())
        {
            if (!_WaitRelease && InputSystem::Instance().GetMouseHold(MOUSE_BUTTON_TYPE::LButton))
            {
                Sprite->ChangeAnimation("BOSS_ENTER.main.realMode_wnd.button_entry.pressed.0");
            }
            else
            {
                Sprite->ChangeAnimation("BOSS_ENTER.main.realMode_wnd.button_entry.mouseOver.0");
            }
        }
        else
        {
            Sprite->ChangeAnimation("BOSS_ENTER.main.realMode_wnd.button_entry.normal.0");
        }
    }

    Sprite = FindSceneComponent<SpriteComponent>("CloseButton");

    Interaction = FindActorComponent<InteractionComponent>("CloseInteraction");

    if (Sprite && Interaction)
    {
        if (Interaction->IsMouseInsideTarget())
        {
            if (!_WaitRelease && InputSystem::Instance().GetMouseHold(MOUSE_BUTTON_TYPE::LButton))
            {
                Sprite->ChangeAnimation("BOSS_ENTER.main.button_close.pressed.0");
            }
            else
            {
                Sprite->ChangeAnimation("BOSS_ENTER.main.button_close.mouseOver.0");
            }
        }
        else
        {
            Sprite->ChangeAnimation("BOSS_ENTER.main.button_close.normal.0");
        }
    }
}

void LucidEntryDialogUI::Open()
{
    if (IsEnable())
    {
        return;
    }

    _WaitRelease = true;

    SetEnable(true);
}

void LucidEntryDialogUI::Close()
{
    SetEnable(false);

    _WaitRelease = true;
}

void LucidEntryDialogUI::Confirm()
{
    if (!IsEnable())
    {
        return;
    }

    Ptr<LucidEntryNPC> NPC = Lock<LucidEntryNPC>(_NPC);

    if (!NPC || !NPC->IsActive() || !NPC->IsEnable())
    {
        Close();

        return;
    }

    Close();

    // 기존 NPC의 입장 처리를 호출한다.
    NPC->ChangeToLucidPhase1();
}

void LucidEntryDialogUI::Destroy()
{
    UI::Destroy();
}
