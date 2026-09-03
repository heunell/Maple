#include "pch.h"
#include "VortexSphere.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Component/SphereCollisionComponent.h"
#include "Game/Character/Player.h"

VortexSphere::VortexSphere()
{}

VortexSphere::~VortexSphere()
{}

bool VortexSphere::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner)
{
    if (!Skill::Init(Id, Position, Scale, Rotator, Name, Owner))
    {
        return false;
    }

    AddTag("PlayerAttack");

    _CastSprite = CreateSceneComponent<SpriteComponent>("VortexSphereCastSprite");

    if (!_CastSprite)
    {
        return false;
    }

    _CastSprite->AttachToComponent(GetRoot());

    _CastSprite->SetRenderLayerName("SkillFront");

    _CastSprite->AddAnimationSequence("VortexSphere.Cast", false);

    _CastSprite->AddNotify("VortexSphere.Cast", 6, this, &VortexSphere::Fire);

    _CastSprite->AddNotify("VortexSphere.Cast", 11, this, &VortexSphere::FinishCast);

    _CastSprite->SetEnable(false);

    _CoolDown = _Datas.Cooldown;

    return true;
}

void VortexSphere::Collision(float DeltaTime)
{
}

void VortexSphere::Tick(float DeltaTime)
{
    Skill::Tick(DeltaTime);

    if (_IsProjectileActive && _ProjectileRoot)
    {
        _LifeTime += DeltaTime;

        float MoveSpeed = _IsMonsterOverlap ? _Datas.HitMoveSpeed : _Datas.MoveSpeed;

        _ProjectileRoot->AddWorldPositionX(_Direction * MoveSpeed * DeltaTime);

        if (_IsMonsterOverlap)
        {
            _HitTimer += DeltaTime;

            if (_HitTimer >= _Datas.HitInterval)
            {
                Hit(_ProjectileRoot->GetWorldPosition());

                _HitTimer -= _Datas.HitInterval;
            }
        }

        _IsMonsterOverlap = false;

        if (_LifeTime >= _Datas.LifeTime)
        {
            FinishProjectile();
        }
    }

    for (size_t i = 0; i < _HitSprites.size();)
    {
        Ptr<SpriteComponent> HitSprite = _HitSprites[i];

        if (!HitSprite)
        {
            _HitSprites.erase(_HitSprites.begin() + i);

            continue;
        }

        Ptr<Animation2D> Animation = HitSprite->GetAnimation();

        if (Animation && Animation->IsFinished())
        {
            HitSprite->Destroy();

            _HitSprites.erase(_HitSprites.begin() + i);

            continue;
        }

        i++;
    }
}

void VortexSphere::Start()
{
    if (!CanUse() || _IsCasting || _IsProjectileActive)
    {
        return;
    }

    Ptr<Actor> Owner = Lock<Actor>(_Owner);

    if (!Owner || !_CastSprite)
    {
        return;
    }

    Ptr<Player> OwnerPlayer = Cast<Actor, Player>(Owner);

    if (!OwnerPlayer)
    {
        return;
    }

    _IsCasting = true;

    StartCoolDown();

    _Direction = OwnerPlayer->IsRight() ? 1.f : -1.f;

    _CastSprite->SetWorldPosition(Owner->GetWorldPosition());

    _CastSprite->SetAnimationFlip(OwnerPlayer->IsRight());

    _CastSprite->SetEnable(true);

    _CastSprite->ChangeAnimation("VortexSphere.Cast");
}

void VortexSphere::Update(float DeltaTime)
{
    if (!_IsCasting)
    {
        return;
    }

    Ptr<Actor> Owner = Lock<Actor>(_Owner);

    if (!Owner || !_CastSprite)
    {
        return;
    }

    _CastSprite->SetWorldPosition(Owner->GetWorldPosition());
}

void VortexSphere::End()
{}

void VortexSphere::Destroy()
{
    FinishProjectile();

    for (Ptr<SpriteComponent>& HitSprite : _HitSprites)
    {
        if (HitSprite)
        {
            HitSprite->Destroy();
        }
    }

    _HitSprites.clear();

    DESTROY(_CastSprite);

    Skill::Destroy();
}

bool VortexSphere::IsCasting() const
{
    return _IsCasting;
}

void VortexSphere::Fire()
{
    if (_IsProjectileActive)
    {
        return;
    }

    Ptr<Actor> Owner = Lock<Actor>(_Owner);

    if (!Owner)
    {
        return;
    }

    Ptr<Player> OwnerPlayer = Cast<Actor, Player>(Owner);

    if (!OwnerPlayer)
    {
        return;
    }

    _ProjectileRoot = CreateSceneComponent<SceneComponent>("VortexSphereProjectileRoot");

    if (!_ProjectileRoot)
    {
        return;
    }

    _ProjectileRoot->AttachToComponent(GetRoot());

    FVector3D SpawnPosition = Owner->GetWorldPosition();

    SpawnPosition += OwnerPlayer->IsRight() ? _Datas.RightPosition : _Datas.LeftPosition;

    _ProjectileRoot->SetWorldPosition(SpawnPosition);

    _ProjectileSprite = CreateSceneComponent<SpriteComponent>("VortexSphereProjectileSprite");

    if (!_ProjectileSprite)
    {
        FinishProjectile();

        return;
    }

    _ProjectileSprite->AttachToComponent(_ProjectileRoot);

    _ProjectileSprite->SetRenderLayerName("SkillFront");

    _ProjectileSprite->AddAnimationSequence("VortexSphere.Projectile", true);

    _ProjectileSprite->SetAnimationFlip(OwnerPlayer->IsRight());

    _ProjectileSprite->ChangeAnimation("VortexSphere.Projectile");

    _ProjectileCollision = CreateSceneComponent<SphereCollisionComponent>("VortexSphereProjectileCollision");

    if (!_ProjectileCollision)
    {
        FinishProjectile();

        return;
    }

    _ProjectileCollision->AttachToComponent(_ProjectileRoot);

    _ProjectileCollision->SetRadius(_Datas.CollisionRadius);

    _ProjectileCollision->SetCollisionProfile("PlayerAttack");

    _ProjectileCollision->SetCollisionCallBack(eCollisionState::COLLISION_STATE_OVERLAP, this, &VortexSphere::OnProjectileOverlap);

    _LifeTime = 0.f;

    _HitTimer = _Datas.HitInterval;

    _IsProjectileActive = true;
}

void VortexSphere::Hit(const FVector3D& Position)
{
    Ptr<SpriteComponent> HitSprite = CreateSceneComponent<SpriteComponent>("VortexSphereHitSprite");

    if (!HitSprite)
    {
        return;
    }

    HitSprite->AttachToComponent(GetRoot());

    HitSprite->SetWorldPosition(Position);

    HitSprite->SetRenderLayerName("SkillFront");

    HitSprite->AddAnimationSequence("VortexSphere.Hit", false);

    HitSprite->ChangeAnimation("VortexSphere.Hit");

    _HitSprites.push_back(HitSprite);
}

void VortexSphere::FinishCast()
{
    _IsCasting = false;

    if (_CastSprite)
    {
        _CastSprite->SetEnable(false);
    }
}

void VortexSphere::FinishProjectile()
{
    if (_ProjectileRoot)
    {
        _ProjectileRoot->Destroy();
    }

    _ProjectileCollision = nullptr;

    _ProjectileSprite = nullptr;

    _ProjectileRoot = nullptr;

    _LifeTime = 0.f;

    _HitTimer = 0.f;

    _IsMonsterOverlap = false;

    _IsProjectileActive = false;
}

void VortexSphere::OnProjectileOverlap(Weak<CollisionComponent> Destination)
{
    Ptr<CollisionComponent> MonsterCollision = Lock<CollisionComponent>(Destination);

    if (!MonsterCollision)
    {
        return;
    }

    Ptr<Actor> Monster = Lock<Actor>(MonsterCollision->GetOwner());

    if (!Monster || !Monster->IsTag("Monster"))
    {
        return;
    }

    _IsMonsterOverlap = true;
}