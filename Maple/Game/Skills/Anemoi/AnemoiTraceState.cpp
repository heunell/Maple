#include "pch.h"
#include "AnemoiTraceState.h"

#include "AnemoiActor.h"
#include "AnemoiAttackState.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"

void AnemoiTraceState::Enter(Ptr<AnemoiActor> Actor)
{
    if (!Actor)
    {
        return;
    }

    Ptr<SpriteComponent> Sprite = Actor->GetSprite();

    Ptr<AABBCollisionComponent> AttackRange = Actor->GetAttackRange();

    if (Sprite)
    {
        Sprite->ChangeAnimation("Anemoi.Stand");
    }

    if (AttackRange)
    {
        AttackRange->SetEnable(true);
    }

    _AttackTimer = 0.f;
}

Ptr<AnemoiState> AnemoiTraceState::Tick(Ptr<AnemoiActor> Actor, float DeltaTime)
{
    if (!Actor)
    {
        return nullptr;
    }

    Ptr<Player> Owner = Actor->GetOwner();

    if (!Owner)
    {
        return nullptr;
    }

    const FAnemoiDatas& Datas = Actor->GetDatas();

    Ptr<SpriteComponent> Sprite = Actor->GetSprite();

    if (Sprite)
    {
        Sprite->SetAnimationFlip(Owner->IsRight());
    }

    const FVector3D TracePosition = Owner->GetWorldPosition() + (Owner->IsRight() ? Datas.TraceRightPosition : Datas.TraceLeftPosition);

    const FVector3D CurrentPosition = Actor->GetWorldPosition();

    FVector3D Direction = TracePosition - CurrentPosition;

    const float Distance = Direction.Length();

    if (Distance >= Datas.TraceSnapDistance)
    {
        Actor->SetWorldPosition(TracePosition);
    }
    else if (Distance > Datas.TraceStopDistance)
    {
        Direction.Normalize();

        const float MoveDistance = Datas.TraceSpeed * DeltaTime;

        if (MoveDistance >= Distance)
        {
            Actor->SetWorldPosition(TracePosition);
        }
        else
        {
            Actor->AddWorldPosition(Direction * MoveDistance);
        }
    }

    _AttackTimer += DeltaTime;

    const float TraceDistance = Actor->GetWorldPosition().Distance(TracePosition);

    if (TraceDistance > Datas.AttackStartDistance)
    {
        return nullptr;
    }

    if (!Actor->GetTarget())
    {
        return nullptr;
    }

    if (_AttackTimer < Datas.AttackInterval)
    {
        return nullptr;
    }

    return New<AnemoiAttackState>();
}