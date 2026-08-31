#include "pch.h"
#include "InteractionComponent.h"

#include "Component/SceneComponent.h"
#include "Input/InputSystem.h"
#include "Object/Actor.h"

#include <cmath>
#include <Windows.h>


void InteractionComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);

    TryInteraction();
}

void InteractionComponent::Destroy()
{
    _InteractionCallback = nullptr;
    _InteractionTarget.reset();
    _LastClickTime = {};

    ActorComponent::Destroy();
}

void InteractionComponent::SetInteractionTarget(const Ptr<class SceneComponent>& Target)
{
    _InteractionTarget = Target;
}

void InteractionComponent::SetInteractionSpace(eInteractionSpace Space)
{
    _Space = Space;
}

void InteractionComponent::SetClickType(eInteractionClickType ClickType)
{
    _ClickType = ClickType;
}

bool InteractionComponent::IsMouseInsideTarget() const
{
    Ptr<SceneComponent> Target = Lock<SceneComponent>(_InteractionTarget);

    if (!Target)
    {
        return false;
    }

    const FVector2D& MousePosition =  _Space == eInteractionSpace::Screen ? InputSystem::Instance().GetMousePosition() : InputSystem::Instance().GetMouseWorldPosition();

    const FVector3D& TargetPosition = Target->GetWorldPosition();

    const FVector3D& TargetScale = Target->GetWorldScale();

    const float HalfWidth = std::abs(TargetScale._x) * 0.5f;

    const float HalfHeight = std::abs(TargetScale._y) * 0.5f;

    return
        MousePosition._x >= TargetPosition._x - HalfWidth &&
        MousePosition._x <= TargetPosition._x + HalfWidth &&
        MousePosition._y >= TargetPosition._y - HalfHeight &&
        MousePosition._y <= TargetPosition._y + HalfHeight;
}

void InteractionComponent::TryInteraction()
{
    if (!InputSystem::Instance().GetMouseDown(MOUSE_BUTTON_TYPE::LButton))
    {
        return;
    }

    // 대상 밖을 클릭하면 기존 첫 번째 클릭을 취소한다.
    if (!IsMouseInsideTarget())
    {
        _LastClickTime = {};
     
        return;
    }

    if (_ClickType == eInteractionClickType::Click)
    {
        if (_InteractionCallback)
        {
            _InteractionCallback();
        }

        return;
    }

    const auto CurrentTime = std::chrono::steady_clock::now();

    const auto EmptyTime = std::chrono::steady_clock::time_point{};

    const auto DoubleClickInterval = std::chrono::milliseconds(GetDoubleClickTime());

    if (_LastClickTime != EmptyTime && CurrentTime - _LastClickTime <= DoubleClickInterval)
    {
        _LastClickTime = {};

        if (_InteractionCallback)
        {
            _InteractionCallback();
        }

        return;
    }

    _LastClickTime = CurrentTime;
}