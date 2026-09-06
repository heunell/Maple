#include "pch.h"
#include "DamageNumber.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Core/AnimationManager.h"
#include "Core/Animation2DData.h"
#include "Core/AssetManager.h"
#include <string>

bool DamageNumber::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, int64 Damage)
{
    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    if (Damage < 0)
    {
        return false;
    }

    std::string Text = std::to_string(Damage);
    float Width = 0.f;

    for (char Digit : Text)
    {
        Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(std::string("LUCID_EXTRA.DAMAGE.NO_RED.") + Digit);

        if (!Data || Data->GetFrameCount() <= 0)
        {
            return false;
        }

        Width += Data->GetFrame(0)._Size._x;
    }

    Width -= static_cast<float>(Text.size() - 1) * 5.f;

    float OffsetX = -Width * 0.5f;

    for (size_t Index = 0; Index < Text.size(); ++Index)
    {
        Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(std::string("LUCID_EXTRA.DAMAGE.NO_RED.") + Text[Index]);

        Ptr<SceneComponent> Slot = CreateSceneComponent<SceneComponent>("DigitSlot" + std::to_string(Index));

        if (!Slot)
        {
            return false;
        }

        Slot->AttachToComponent(GetRoot());

        Slot->SetRelativePosition(OffsetX + Data->GetFrame(0)._Size._x * 0.5f - Data->GetFrame(0)._Offset._x, -Data->GetFrame(0)._Offset._y, 0.f);

        Ptr<SpriteComponent> Sprite = CreateSceneComponent<SpriteComponent>("Digit" + std::to_string(Index));

        if (!Sprite)
        {
            return false;
        }

        Sprite->AttachToComponent(Slot);

        Sprite->SetRenderLayerName("SkillFront");

        Sprite->AddAnimationSequence(Data, false);

        OffsetX += Data->GetFrame(0)._Size._x - 5.f;
    }

    return true;
}

void DamageNumber::Tick(float DeltaTime)
{
    _ElapsedTime += DeltaTime;

    if (_ElapsedTime >= 0.7f)
    {
        Destroy();

        Remove();

        return;
    }

    Actor::Tick(DeltaTime);
}

void DamageNumber::Destroy()
{
    SetEnable(false);

    SetActive(false);

    if (GetRoot())
    {
        GetRoot()->Destroy();
    }

    Actor::Destroy();
}