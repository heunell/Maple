#include "pch.h"
#include "PlayerUI.h"
#include "ProgressBar.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"

PlayerUI::PlayerUI()
{}

PlayerUI::~PlayerUI()
{}



bool PlayerUI::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!UI::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    if (!CreateSprite("MainStatusBack", "UI_MAIN_STATUS_BACK"))
    {
        return false;
    }

    _HPBar = CreateProgressBar("HPBar", "UI_HP_GAUGE");

    if(!_HPBar)
    {
        return false;
    }

    _MPBar = CreateProgressBar("MPBar", "UI_MP_GAUGE");

    if (!_MPBar)
    {
        return false;
    }

    if (!CreateSprite("MainStatusCover", "UI_MAIN_STATUS_COVER"))
    {
        return false;
    }

    if (!CreateSprite("EXPBarBack", "UI_EXP_BAR_BACK"))
    {
        return false;
    }

    _EXPBar = CreateProgressBar("EXPBar", "UI_EXP_BAR_GAUGE");

    if (!_EXPBar)
    {
        return false;
    }

    if (!CreateSprite("QuickSlotBack", "UI_QUICK_SLOT_BACK"))
    {
        return false;
    }

    SetHP(1.f, 1.f);

    SetMP(1.f, 1.f);

    SetEXP(0.f, 1.f);

    return true;
}


Ptr<class SpriteComponent> PlayerUI::CreateSprite(const std::string& ComponentName, const std::string& AnimationName)
{
    Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(AnimationName);

    if (!Data)
    {
        return nullptr;
    }

    Ptr<SpriteComponent> Sprite = CreateSceneComponent<SpriteComponent>(ComponentName);

    if (!Sprite)
    {
        return nullptr;
    }

    Sprite->SetRenderLayerName("UI");

    Sprite->AddAnimationSequence(Data);
    
    Sprite->AttachToComponent(GetRoot());

    return Sprite;
}

Ptr<class ProgressBar> PlayerUI::CreateProgressBar(const std::string& ComponentName, const std::string& AnimationName)
{
    Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(AnimationName);

    if (!Data)
    {
        return nullptr;
    }

    Ptr<ProgressBar> Bar = CreateSceneComponent<ProgressBar>(ComponentName);

    if (!Bar)
    {
        return nullptr;
    }

    Bar->SetRenderLayerName("UI");

    Bar->AttachToComponent(GetRoot());

    if (!Bar->SetData(Data))
    {
        return nullptr;
    }

    return Bar;
}

void PlayerUI::SetHP(float CurrentHP, float MaxHP)
{
    if (_HPBar)
    {
        _HPBar->SetValue(CurrentHP, MaxHP);
    }
}

void PlayerUI::SetMP(float CurrentMP, float MaxMP)
{
    if (_MPBar)
    {
        _MPBar->SetValue(CurrentMP, MaxMP);
    }
}

void PlayerUI::SetEXP(float CurrentEXP, float MaxEXP)
{
    if (_EXPBar)
    {
        _EXPBar->SetValue(CurrentEXP, MaxEXP);
    }
}

void PlayerUI::Destroy()
{
    _HPBar = nullptr;

    _MPBar = nullptr;
    
    _EXPBar = nullptr;

    UI::Destroy();
}
