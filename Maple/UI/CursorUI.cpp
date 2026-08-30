#include "pch.h"
#include "CursorUI.h"
#include "Component/SpriteComponent.h"
#include "Core/Animation2DData.h"
#include "Core/AnimationManager.h"
#include "Core/AssetManager.h"
#include "Input/InputSystem.h"

bool CursorUI::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if (!UI::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    _CursorSprite = CreateSceneComponent<SpriteComponent>("CursorSprite");

    if (!_CursorSprite)
    {
        return false;
    }

    _CursorSprite->SetRenderLayerName("Cursor");

    _CursorSprite->AttachToComponent(GetRoot());

    if (!AddCursorAnimation("Cursor.Normal"))
    {
        return false;
    }

    if (!AddCursorAnimation("Cursor.LeftPressed"))
    {
        return false;
    }

    _CursorSprite->ChangeAnimation("Cursor.Normal");

    return true;
}

void CursorUI::Tick(float DeltaTime)
{
    UI::Tick(DeltaTime);

    if (!_CursorSprite)
    {
        return;
    }

    const FVector2D& MousePosition = InputSystem::Instance().GetMousePosition();

    SetWorldPosition(MousePosition._x, MousePosition._y, GetWorldPosition()._z);

    if (InputSystem::Instance().GetMouseHold(MOUSE_BUTTON_TYPE::LButton))
    {
        _CursorSprite->ChangeAnimation("Cursor.LeftPressed");
    }
    else
    {
        _CursorSprite->ChangeAnimation("Cursor.Normal");
    }
}

void CursorUI::Destroy()
{
    _CursorSprite = nullptr;

    UI::Destroy();
}

bool CursorUI::AddCursorAnimation(const std::string& AnimationName, bool Loop)
{
    Ptr<Animation2DData> Data = ANIMATION_MANAGER->FindAnimation(AnimationName);

    if (!Data)
    {
        return false;
    }

    _CursorSprite->AddAnimationSequence(Data, Loop);

    return true;
}
