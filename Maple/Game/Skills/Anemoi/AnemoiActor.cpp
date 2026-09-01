#include "pch.h"
#include "AnemoiActor.h"
#include "AnemoiStateMachine.h"
#include "AnemoiIdleState.h"
#include "AnemoiEndState.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"

void AnemoiActor::OnTargetOverlap(Weak<class CollisionComponent> Destination)
{
    if(Lock(_Target))
    {
        return;
    }

    Ptr<CollisionComponent> TargetCollision = Lock(Destination);

    if(!TargetCollision)
    {
        return;
    }

    Ptr<Actor> Target = Lock<Actor>(TargetCollision->GetOwner());

    if(!Target || !Target->IsTag("Monster"))
    {
        return;
    }

    _Target = Target;
}

void AnemoiActor::OnTargetRelease(Weak<class CollisionComponent> Destination)
{
    Ptr<CollisionComponent> TargetCollision = Lock(Destination);

    if(!TargetCollision)
    {
        return;
    }

    Ptr<Actor> ReleasedTarget = Lock<Actor>(TargetCollision->GetOwner());

    if (ReleasedTarget == Lock(_Target))
    {
        _Target.reset();
    }
}

bool AnemoiActor::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<Player> Owner, const FAnemoiDatas& Datas)
{
    if (!Owner)
    {
        return false;
    }

    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    _Owner = Owner;

    _Datas = Datas;

    _Sprite = CreateSceneComponent<SpriteComponent>("AnemoiSprite");

    _Sprite->AttachToComponent(GetRoot());

    _Sprite->AddAnimationSequence("Anemoi.Summon", false);

    _Sprite->AddAnimationSequence("Anemoi.Stand", true);

    _Sprite->AddAnimationSequence("Anemoi.Attack", false);

    _Sprite->AddAnimationSequence("Anemoi.Die", false);

    _AttackRange = CreateSceneComponent<AABBCollisionComponent>("AnemoiAttackRange");

    _AttackRange->AttachToComponent(GetRoot());

    _AttackRange->SetCollisionCallBack(eCollisionState::COLLISION_STATE_OVERLAP, this, &AnemoiActor::OnTargetOverlap);

    _AttackRange->SetCollisionCallBack(eCollisionState::COLLISION_STATE_RELEASE, this, &AnemoiActor::OnTargetRelease);
    
    _AttackRange->SetBoxSize(_Datas.AttackRangeWidth, _Datas.AttackRangeHeight);

    _AttackRange->SetRelativePosition(0.f, _Datas.AttackRangeOffsetY, 0.f);

    _AttackRange->SetCollisionProfile("PlayerAttack");

    _AttackRange->SetEnable(false);

    _StateMachine = New<AnemoiStateMachine>();

    _StateMachine->Init(This<AnemoiActor>(), New<AnemoiIdleState>());

    return true;
}

void AnemoiActor::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);

    const bool LifeTimeEnded = _CurrentLifeTime < _Datas.LifeTime && _CurrentLifeTime + DeltaTime >= _Datas.LifeTime;

    _CurrentLifeTime += DeltaTime;

    if (LifeTimeEnded)
    {
        _StateMachine->Transition(New<AnemoiEndState>());

        return;
    }

    if (_StateMachine)
    {
        _StateMachine->Tick(DeltaTime);
    }
}

void AnemoiActor::Destroy()
{
    DESTROY(_StateMachine);

    _AttackRange.reset();

    _Sprite.reset();

    _Owner.reset();

    _Target.reset();
    
    Actor::Destroy();
}

Ptr<Player> AnemoiActor::GetOwner() const
{
    return Lock(_Owner);
}

Ptr<Actor> AnemoiActor::GetTarget() const
{
    return Lock(_Target);
}

Ptr<SpriteComponent> AnemoiActor::GetSprite() const
{
    return _Sprite;
}

Ptr<AABBCollisionComponent> AnemoiActor::GetAttackRange() const
{
    return _AttackRange;
}

Ptr<AnemoiStateMachine> AnemoiActor::GetStateMachine() const
{
    return _StateMachine;
}

const FAnemoiDatas& AnemoiActor::GetDatas() const
{
    return _Datas;
}