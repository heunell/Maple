#include "pch.h"
#include "UI.h"
#include "ProgressBar.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "Core/Device.h"

bool UI::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    const FResolution& Resolution = Device::Instance().GetRS();

    // UI의 (0, 0)은 화면 중앙을 기준으로 사용한다.
    const FVector3D ScreenPosition(static_cast<float>(Resolution._Width) * 0.5f + Position._x, static_cast<float>(Resolution._Height) * 0.5f + Position._y, Position._z);

    return Actor::Init(Id, ScreenPosition, Scale, Rotator, Name);
}

Ptr<class SpriteComponent> UI::CreateSprite(const std::string &ComponentName, const std::string &AnimationName, Ptr<class SceneComponent> Parent)
{
    Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(AnimationName);

    if(!Data)
    {
        return nullptr;
    }

    Ptr<SpriteComponent> Sprite = CreateSceneComponent<SpriteComponent>(ComponentName);

    if(!Sprite)
    {
        return nullptr;
    }

    Sprite->SetRenderLayerName("UI");

    Sprite->AddAnimationSequence(Data);

    if (Parent)
    {
        Sprite->AttachToComponent(Parent);
    }
    else
    {
        Sprite->AttachToComponent(GetRoot());
    }

    return Sprite;
}

Ptr<class ProgressBar> UI::CreateProgressBar(const std::string &ComponentName, const std::string &AnimationName)
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