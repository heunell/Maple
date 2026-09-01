#include "pch.h"
#include "AnemoiIdleState.h"
#include "AnemoiActor.h"
#include "AnemoiTraceState.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"

void AnemoiIdleState::Enter(Ptr<AnemoiActor> Actor)
{
    if (!Actor)
    {
        return;
    }

    Ptr<Player> Owner = Actor->GetOwner();

    Ptr<SpriteComponent> Sprite = Actor->GetSprite();

    if (!Owner || !Sprite)
    {
        return;
    }

    Sprite->SetAnimationFlip(Owner->IsRight());

    Sprite->SetEnable(true);

    Sprite->ChangeAnimation("Anemoi.Summon");
}

Ptr<AnemoiState> AnemoiIdleState::Tick(Ptr<AnemoiActor> Actor, float DeltaTime)
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

    Ptr<Animation2D> Animation = Sprite->GetAnimation();

    if (Animation && Animation->IsFinished())
    {
        return New<AnemoiTraceState>();
    }

    return nullptr;
}