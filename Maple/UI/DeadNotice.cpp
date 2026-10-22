#include "pch.h"
#include "DeadNotice.h"
#include "ProgressBar.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/InteractionComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "Game/Character/Player.h"
#include "World/GameLevel.h"
#include "Input/InputSystem.h"

bool DeadNotice::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<class Player> Player)
{
    if (!UI::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    if (!Player || Player->GetHP() > 0)
    {
        return false;
    }

    Ptr<GameLevel> CurrentLevel = Cast<Level, GameLevel>(GetLevel());

    if (!CurrentLevel)
    {
        return false;
    }

    _Player = Player;
    
    _ElapsedTime = 0.f;
    
    _WaitRelease = true;

    if (!CreateSprite("DeadBackground", "DeathNoticeUIFrame.background_full"))
    {
        return false;
    }

    Ptr<SceneComponent> Slot = CreateSceneComponent<SceneComponent>("DeadMessageSlot");

    if (!Slot)
    {
        return false;
    }

    Slot->AttachToComponent(GetRoot());

    if (CurrentLevel->GetBossDeathCount() > 0)
    {
        if (!CreateSprite("DeadMessage", "DeathNoticeUIFrame.top_noTransfer", Slot))
        {
            return false;
        }
    }
    else
    {
        Slot->SetRelativePosition(0.f, -37.f, 0.f);

        if (!CreateSprite("DeadMessage", "DeathNoticeUIFrame.autoRevive_msgDeathcount", Slot))
        {
            return false;
        }
    }

    Slot = CreateSceneComponent<SceneComponent>("DeadProgressSlot");

    if (!Slot)
    {
        return false;
    }

    Slot->AttachToComponent(GetRoot());
    
    Slot->SetRelativePosition(0.f, -109.f, 0.f);

    if (CurrentLevel->GetBossDeathCount() > 0)
    {
        Ptr<SpriteComponent> Message = CreateSprite("DeadCountdownMessage", "DeathNoticeUIFrame.autoRevive_msgWaitUser", Slot);

        if (!Message)
        {
            return false;
        }

        Message->AddAnimationSequence("DeathNoticeUIFrame.autoRevive_msgDelay");
    }

    if (!CreateSprite("DeadProgressBackground", "DeathNoticeUIFrame.autoRevive_timerGaugeBack", Slot))
    {
        return false;
    }

    Ptr<ProgressBar> Bar = CreateSceneComponent<ProgressBar>("DeadProgress");

    if (!Bar)
    {
        return false;
    }

    Bar->AttachToComponent(Slot);
    
    Bar->SetRenderLayerName("UI");

    Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation("DeathNoticeUIFrame.autoRevive_timerGauge");

    if (!Bar->SetData(Data))
    {
        return false;
    }

    Bar->SetValue(30.f, 30.f);

    Ptr<SceneComponent> EdgeSlot = CreateSceneComponent<SceneComponent>("DeadProgressEdgeSlot");

    if (!EdgeSlot)
    {
        return false;
    }

    EdgeSlot->AttachToComponent(Slot);
    
    EdgeSlot->SetRelativePosition(396.f, -60.f, 0.f);

    if (!CreateSprite("DeadProgressEdge", "DeathNoticeUIFrame.autoRevive_timerGaugeEdge", EdgeSlot))
    {
        return false;
    }

    Slot = CreateSceneComponent<SceneComponent>("DeadConfirmSlot");

    if (!Slot)
    {
        return false;
    }

    Slot->AttachToComponent(GetRoot());
    
    Slot->SetRelativePosition(46.f, -162.f, 0.f);

    Ptr<SpriteComponent> Button = CreateSprite("DeadConfirm", "DeathNoticeUIFrame.btYes_normal_0", Slot);

    if (!Button)
    {
        return false;
    }

    Button->AddAnimationSequence("DeathNoticeUIFrame.btYes_mouseOver_0");

    Button->AddAnimationSequence("DeathNoticeUIFrame.btYes_pressed_0");

    Button->AddAnimationSequence("DeathNoticeUIFrame.btYes_disabled_0");

    Ptr<InteractionComponent> Interaction = CreateActorComponent<InteractionComponent>("DeadConfirmInteraction");

    if (!Interaction)
    {
        return false;
    }

    Interaction->SetInteractionTarget(Button);
    
    Interaction->SetInteractionSpace(eInteractionSpace::Screen);
    
    Interaction->SetClickType(eInteractionClickType::Click);
    
    Interaction->SetInteractionCallback(this, &DeadNotice::Confirm);

    // if (CurrentLevel->GetBossDeathCount() <= 0)
    // {
    //     Interaction->SetEnable(false);
    //
    //     Button->ChangeAnimation("DeathNoticeUIFrame.btYes_disabled_0");
    // }

    return true;
}

void DeadNotice::Tick(float DeltaTime)
{
    Ptr<Player> Player = Lock<class Player>(_Player);

    Ptr<GameLevel> CurrentLevel = Cast<Level, GameLevel>(GetLevel());

    if (!Player || !CurrentLevel || Player->GetHP() > 0)
    {
        Destroy();
        
        Remove();
        
        return;
    }

    _ElapsedTime += DeltaTime;

    if (CurrentLevel->GetBossDeathCount() <= 0)
    {
        if (_ElapsedTime >= 30.f)
        {
            if (CurrentLevel->EndBossBattle())
            {
                Player->Revive();
            }

            return;
        }
    }
    else if (_ElapsedTime >= 35.f)
    {
        Confirm();
        
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

    float RemainingTime = 30.f - _ElapsedTime;
    
    float MaxTime = 30.f;

    if (CurrentLevel->GetBossDeathCount() > 0 && _ElapsedTime >= 30.f)
    {
        RemainingTime = 35.f - _ElapsedTime;
    
        MaxTime = 5.f;

        Ptr<SpriteComponent> Message = FindSceneComponent<SpriteComponent>("DeadCountdownMessage");

        if (Message)
        {
            Message->ChangeAnimation("DeathNoticeUIFrame.autoRevive_msgDelay");
        }
    }

    if (RemainingTime < 0.f)
    {
        RemainingTime = 0.f;
    }

    Ptr<ProgressBar> Bar = FindSceneComponent<ProgressBar>("DeadProgress");

    if (Bar)
    {
        Bar->SetValue(RemainingTime, MaxTime);
    }

    Ptr<SceneComponent> EdgeSlot = FindSceneComponent<SceneComponent>("DeadProgressEdgeSlot");

    if (EdgeSlot)
    {
        EdgeSlot->SetRelativePosition(30.f + 366.f * RemainingTime / MaxTime, -60.f, 0.f);
    }

    Ptr<SpriteComponent> Button = FindSceneComponent<SpriteComponent>("DeadConfirm");

    Ptr<InteractionComponent> Interaction = FindActorComponent<InteractionComponent>("DeadConfirmInteraction");

    if (!Button || !Interaction)
    {
        return;
    }

    // if (CurrentLevel->GetBossDeathCount() <= 0)
    // {
    //     Button->ChangeAnimation("DeathNoticeUIFrame.btYes_disabled_0");
    //
    //     return;
    // }

    if (Interaction->IsMouseInsideTarget())
    {
        if (!_WaitRelease && InputSystem::Instance().GetMouseHold(MOUSE_BUTTON_TYPE::LButton))
        {
            Button->ChangeAnimation("DeathNoticeUIFrame.btYes_pressed_0");
        }
        else
        {
            Button->ChangeAnimation("DeathNoticeUIFrame.btYes_mouseOver_0");
        }
    }
    else
    {
        Button->ChangeAnimation("DeathNoticeUIFrame.btYes_normal_0");
    }
}

void DeadNotice::Confirm()
{
    if (!IsEnable())
    {
        return;
    }

    Ptr<Player> Player = Lock<class Player>(_Player);

    Ptr<GameLevel> CurrentLevel = Cast<Level, GameLevel>(GetLevel());

    if (!Player || !CurrentLevel)
    {
        return;
    }

    if (CurrentLevel->GetBossDeathCount() <= 0)
    {
        if (!CurrentLevel->EndBossBattle())
        {
            return;
        }
    }
    
    Player->Revive();
}

void DeadNotice::Destroy()
{
    SetEnable(false);
    
    SetActive(false);

    if (GetRoot())
    {
        GetRoot()->Destroy();
    }

    _Player.reset();

    UI::Destroy();
}