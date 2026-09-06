#pragma once
#include "Object/Actor.h"

class TriflingWindActor : public Actor
{
public:
    TriflingWindActor() = default;
    virtual ~TriflingWindActor() = default;

private:
    Ptr<class SpriteComponent> _Sprite;

    Weak<class MonsterBase> _Target;

    FVector2D _Direction = FVector2D(1.f, 0.f);

    float _Angle = 0.f;

    float _TurnResponse = 0.55f;

    float _ElapsedTime = 0.f;

    bool _Enhanced = false;

    bool _FacingRight = true;

    bool _Dying = false;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;

    virtual void Tick(float DeltaTime) override;

    virtual void Destroy() override;

    void Start(Ptr<class MonsterBase> Target, bool Enhanced, float Angle);

private:
    Ptr<class MonsterBase> FindTarget() const;

    void UpdateMovement(float DeltaTime);

    void UpdateVisual();

    void Hit();

    void End();
};