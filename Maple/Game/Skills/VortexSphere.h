#pragma once

#include "Skill.h"
#include <vector>

struct FVortexSphereDatas
{
    float Cooldown = 30.f;           // 스킬 쿨타임
    float LifeTime = 20.f;           // 투사체 유지시간
    float MoveSpeed = 150.f;         // 기본 이동속도
    float HitMoveSpeed = 75.f;       // 몬스터 타격 중 이동속도
    float CollisionRadius = 150.f;   // 공격 충돌 반지름
    float HitInterval = 0.23f;       // 연속 공격 간격
    int32 DamageLines = 6;           // 한 번 공격할 때 출력할 데미지 줄 수
    FVector3D RightPosition = FVector3D(100.f, 0.f, 0.f); // 오른쪽 발사 위치
    FVector3D LeftPosition = FVector3D(-100.f, 0.f, 0.f); // 왼쪽 발사 위치
};

class VortexSphere : public Skill
{
public:
    VortexSphere();
    virtual ~VortexSphere();

private:
    FVortexSphereDatas _Datas;

    Ptr<class SpriteComponent> _CastSprite;

    Ptr<class SceneComponent> _ProjectileRoot;

    Ptr<class SpriteComponent> _ProjectileSprite;

    Ptr<class SphereCollisionComponent> _ProjectileCollision;

    std::vector<Ptr<class SpriteComponent>> _HitSprites;

    float _LifeTime = 0.f;

    float _HitTimer = 0.f;

    float _Direction = 1.f;

    bool _IsCasting = false;

    bool _IsProjectileActive = false;

    bool _IsMonsterOverlap = false;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner) override;

    virtual void Collision(float DeltaTime) override;

    virtual void Tick(float DeltaTime) override;

    virtual void Start() override;

    virtual void Update(float DeltaTime) override;

    virtual void End() override;

    virtual void Destroy() override;

    bool IsCasting() const;

private:
    void Fire();

    void Hit(const FVector3D& Position);

    void FinishCast();

    void FinishProjectile();

    void OnProjectileOverlap(Weak<class CollisionComponent> Destination);
};