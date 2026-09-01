#include "pch.h"
#include "AnemoiEndState.h"
#include "AnemoiActor.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"

void AnemoiEndState::Enter(Ptr<AnemoiActor> Actor)
{
    if (!Actor)
    {
        return;
    }

    Ptr<Player> Owner = Actor->GetOwner();

    Ptr<SpriteComponent> Sprite = Actor->GetSprite();

    Ptr<AABBCollisionComponent> AttackRange = Actor->GetAttackRange();

    if (AttackRange)
    {
        AttackRange->SetEnable(false);
    }

    if (!Owner || !Sprite)
    {
        return;
    }

    Sprite->SetAnimationFlip(Owner->IsRight());

    Sprite->ChangeAnimation("Anemoi.Die");
}

Ptr<AnemoiState> AnemoiEndState::Tick(Ptr<AnemoiActor> Actor, float DeltaTime)
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
        Actor->Remove();
    }

    return nullptr;
}