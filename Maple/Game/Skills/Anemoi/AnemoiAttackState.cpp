#include "pch.h"
#include "AnemoiAttackState.h"

#include "AnemoiActor.h"
#include "AnemoiTraceState.h"
#include "Component/SpriteComponent.h"

void AnemoiAttackState::Enter(Ptr<AnemoiActor> Actor)
{
    if (!Actor)
    {
        return;
    }

    Ptr<SpriteComponent> Sprite = Actor->GetSprite();

    if (!Sprite)
    {
        return;
    }

    Ptr<class Actor> Target = Actor->GetTarget();

    if (Target)
    {
        const bool IsRight = Target->GetWorldPosition()._x >= Actor->GetWorldPosition()._x;

        Sprite->SetAnimationFlip(IsRight);
    }

    Sprite->ChangeAnimation("Anemoi.Attack");
}

Ptr<AnemoiState> AnemoiAttackState::Tick(Ptr<AnemoiActor> Actor, float DeltaTime)
{
    if (!Actor)
    {
        return nullptr;
    }

    Ptr<SpriteComponent> Sprite = Actor->GetSprite();

    if (!Sprite)
    {
        return nullptr;
    }

    Ptr<class Actor> Target = Actor->GetTarget();

    if (Target)
    {
        const bool IsRight = Target->GetWorldPosition()._x >= Actor->GetWorldPosition()._x;

        Sprite->SetAnimationFlip(IsRight);
    }

    Ptr<Animation2D> Animation = Sprite->GetAnimation();

    if (Animation && Animation->IsFinished())
    {
        return New<AnemoiTraceState>();
    }

    return nullptr;
}