#pragma once

#include "Game/Skills/Skill.h"
#include "AnemoiDatas.h"

class Anemoi : public Skill
{
public:
    Anemoi() = default;
    virtual ~Anemoi() = default;

private:
    FAnemoiDatas _Datas;

    bool _Casting = false;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<class Actor> Owner) override;

    virtual void Tick(float DeltaTime) override;

    virtual void Collision(float DeltaTime) override;

    virtual void Start() override;

    virtual void Update(float DeltaTime) override;

    virtual void End() override;

    virtual bool IsActive() const override;

    virtual void Destroy() override;
};