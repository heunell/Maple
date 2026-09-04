#pragma once

#include "Object/Actor.h"
#include "HowlingGaleDatas.h"
#include <vector>

class HowlingGaleActor : public Actor
{
public:
    HowlingGaleActor();
    virtual ~HowlingGaleActor();

private:
    Weak<class Player> _Owner;

    FHowlingGaleDatas _Datas;

    Ptr<class SpriteComponent> _Sprite;

    Ptr<class AABBCollisionComponent> _AttackRange;

    std::vector<Ptr<class SpriteComponent>> _HitSprites;

    FVector3D _HitPosition;

    float _CurrentLifeTime = 0.f;

    float _HitTimer = 0.f;

    float _Direction = 1.f;

    int32 _Stack = 1;

    bool _IsMonsterOverlap = false;

    bool _IsEnding = false;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<class Player> Owner, const FHowlingGaleDatas& Datas, int32 Stack, float Direction);

    virtual void Tick(float DeltaTime) override;

    virtual void Destroy() override;

private:
    void OnMonsterOverlap(Weak<class CollisionComponent> Destination);

    void Hit();

    void BeginEnd();
};