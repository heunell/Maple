#pragma once
#include "Object/Actor.h"

class DamageNumber : public Actor
{
public:
    DamageNumber() = default;
    virtual ~DamageNumber() = default;

private:
    float _ElapsedTime = 0.f;

public:
    bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, int64 Damage);

    virtual void Tick(float DeltaTime) override;

    virtual void Destroy() override;
};