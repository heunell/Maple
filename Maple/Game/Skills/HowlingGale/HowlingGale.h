#pragma once

#include "Game/Skills/Skill.h"
#include "HowlingGaleDatas.h"

class HowlingGale : public Skill
{
public:
    HowlingGale();
    virtual ~HowlingGale();

private:
    FHowlingGaleDatas _Datas;

    Ptr<class SpriteComponent> _CastSprite;

    float _CurrentChargeTime = 0.f;

    int32 _Stack = 1;

    float _Direction = 1.f;

    bool _IsCharging = false;

    bool _IsCasting = false;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner) override;

    virtual void Tick(float DeltaTime) override;

    virtual void Collision(float DeltaTime) override;

    virtual void Start() override;

    virtual void Update(float DeltaTime) override;

    virtual void End() override;

    virtual void Destroy() override;

    virtual bool IsActive() const override;

private:
    void Fire();

    void FinishCast();
};