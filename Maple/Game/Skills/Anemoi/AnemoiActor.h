#pragma once
#include "Object/Actor.h"
#include "AnemoiDatas.h"

class AnemoiActor : public Actor
{
public:
    AnemoiActor() = default;
    virtual ~AnemoiActor() = default;

private:
    Weak<class Player> _Owner;

    Weak<class Actor> _Target;

    Ptr<class SpriteComponent> _Sprite = nullptr;

    Ptr<class AABBCollisionComponent> _AttackRange = nullptr;

    Ptr<class AnemoiStateMachine> _StateMachine = nullptr;

    FAnemoiDatas _Datas;

    float _CurrentLifeTime = 0.f;

    void OnTargetOverlap(Weak<class CollisionComponent> Destination);

    void OnTargetRelease(Weak<class CollisionComponent> Destination);

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<class Player> Owner, const FAnemoiDatas& Datas);

    virtual void Tick(float DeltaTime) override;

    virtual void Destroy() override;

    Ptr<class Player> GetOwner() const;

    Ptr<class Actor> GetTarget() const;
    
    Ptr<class SpriteComponent> GetSprite() const;

    Ptr<class AABBCollisionComponent> GetAttackRange() const;

    Ptr<class AnemoiStateMachine> GetStateMachine() const;

    const FAnemoiDatas& GetDatas() const;
};