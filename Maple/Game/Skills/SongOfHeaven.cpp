#include "pch.h"
#include "SongOfHeaven.h"
#include "Object/Actor.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SceneComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"

SongOfHeaven::SongOfHeaven()
{}

SongOfHeaven::~SongOfHeaven()
{}

void SongOfHeaven::NextState()
{
    Ptr<Actor> Owner = Lock<Actor>(_Owner);

    if (!Owner)
    {
        return;
    }

    Ptr<SpriteComponent> SkillSprite = Owner->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (!SkillSprite)
    {
        return; 
    }

    switch (_State)
    {
    case eSongOfHeavenState::Ready:

        _State = eSongOfHeavenState::Start;

        _FireTimer = _FireInterval;

        SkillSprite->ChangeAnimation("SongOfHeaven.KeyDown");

        break;

    case eSongOfHeavenState::Start:

        _State = eSongOfHeavenState::Loop;

        SkillSprite->ChangeAnimation("SongOfHeaven.KeyDownLoop");

        break;

    case eSongOfHeavenState::End:

        _State = eSongOfHeavenState::Idle;

        SkillSprite->SetEnable(false);

        break;

    default:

        break;
    }
}

bool SongOfHeaven::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner)
{
    if (!Skill::Init(Id, Position, Scale, Rotator, Name, Owner))
    {
        return false;
    }

    AddTag("PlayerAttack");

    Ptr<SpriteComponent> SkillSprite = Owner->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (!SkillSprite)
    {
        return false;
    }

    SkillSprite->AddAnimationSequence("SongOfHeaven.Prepare", false);
   
    SkillSprite->AddAnimationSequence("SongOfHeaven.KeyDown", false);
    
    SkillSprite->AddAnimationSequence("SongOfHeaven.KeyDownLoop", true);
    
    SkillSprite->AddAnimationSequence("SongOfHeaven.KeyDownEnd", false);
    
    SkillSprite->AddNotify("SongOfHeaven.Prepare", 3, this, &SongOfHeaven::NextState);

    SkillSprite->AddNotify("SongOfHeaven.KeyDown", 16, this, &SongOfHeaven::NextState);
    
    SkillSprite->AddNotify("SongOfHeaven.KeyDownEnd", 9, this, &SongOfHeaven::NextState);
    
    return true;
}

void SongOfHeaven::Collision(float DeltaTime)
{}

void SongOfHeaven::Tick(float DeltaTime)
{
    Skill::Tick(DeltaTime);

    for (size_t i = 0; i < _Projectiles.size();)
    {
        FSongOfHeavenProjectile& Projectile = _Projectiles[i];

        Projectile.CurrentLifeTime += DeltaTime;

        if (Projectile.CurrentLifeTime >= Projectile.LifeTime || !Projectile.Root || !Projectile.Root->IsActive())
        {
            if (Projectile.Root)
            {
                Projectile.Root->Destroy();
            }

            _Projectiles.erase(_Projectiles.begin() + i);

            continue;
        }

        Projectile.Root->AddWorldPositionX(Projectile.Direction * 500.f * DeltaTime);

        i++;
    }

    for (size_t i = 0; i < _HitEffectSprites.size();)
    {
        Ptr<SpriteComponent> HitEffectSprite = _HitEffectSprites[i];

        if (!HitEffectSprite)
        {
            _HitEffectSprites.erase(_HitEffectSprites.begin() + i);

            continue;
        }

        Ptr<Animation2D> Animation = HitEffectSprite->GetAnimation();

        if (Animation && Animation->IsFinished())
        {
            HitEffectSprite->Destroy();

            _HitEffectSprites.erase(_HitEffectSprites.begin() + i);

            continue;
        }

        ++i;
    }
}

void SongOfHeaven::Start(float DeltaTime)
{
    Ptr<Actor> Owner = Lock<Actor>(_Owner);

    if (!Owner)
    {
        return;
    }

    Ptr<SpriteComponent> PlayerSprite = Owner->FindSceneComponent<SpriteComponent>("PlayerSprite");

    if (!PlayerSprite)
    {
        return;
    }

    Ptr<SpriteComponent> SkillSprite = Owner->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (!SkillSprite)
    {
        return;
    }

    Ptr<Player> OwnerPlayer = Cast<Actor, Player>(Owner);

    if (!OwnerPlayer)
    {
        return;
    }

    if (_State != eSongOfHeavenState::Idle)
    {
        return;
    }

    _State = eSongOfHeavenState::Ready;

    SkillSprite->SetAnimationFlip(OwnerPlayer->IsRight());

    PlayerSprite->ChangeAnimation("ARMED_SHOOT");

    SkillSprite->SetEnable(true);

    SkillSprite->ChangeAnimation("SongOfHeaven.Prepare");

}

void SongOfHeaven::Update(float DeltaTime)
{
    if (_State == eSongOfHeavenState::Idle || _State == eSongOfHeavenState::End)
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

    Ptr<SpriteComponent> PlayerSprite = Owner->FindSceneComponent<SpriteComponent>("PlayerSprite");
    if (PlayerSprite)
    {
        PlayerSprite->ChangeAnimation("ARMED_SHOOT");
    }

    Ptr<SpriteComponent> SkillSprite = Owner->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (SkillSprite)
    {
        SkillSprite->SetAnimationFlip(OwnerPlayer->IsRight());
    }


    _FireTimer += DeltaTime;

    if (_FireTimer >= _FireInterval)
    {
        Fire(DeltaTime);

        _FireTimer -= _FireInterval;
    }
}

void SongOfHeaven::End(float DeltaTime)
{
    if (_State == eSongOfHeavenState::Idle || _State == eSongOfHeavenState::End)
    {
        return;
    }

    Ptr<Actor> Owner = Lock<Actor>(_Owner);

    if (!Owner)
    {
        return;
    }

    Ptr<SpriteComponent> PlayerSprite = Owner->FindSceneComponent<SpriteComponent>("PlayerSprite");

    if (PlayerSprite)
    {
        PlayerSprite->ChangeAnimation("ARMED_STAND");
    }
    
    Ptr<SpriteComponent> SkillSprite = Owner->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (!SkillSprite)
    {
        return;
    }

    _State = eSongOfHeavenState::End;

    SkillSprite->ChangeAnimation("SongOfHeaven.KeyDownEnd");
}

void SongOfHeaven::Destroy()
{
    Skill::Destroy();
}

void SongOfHeaven::Fire(float DeltaTime)
{
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

    Ptr<SceneComponent> ProjectileRoot = CreateSceneComponent<SceneComponent>("ProjectileRoot");

    ProjectileRoot->AttachToComponent(GetRoot());

    ProjectileRoot->SetWorldPosition(Owner->GetWorldPosition());

    ProjectileRoot->SetRelativeScale(1.f, 1.f, 1.f);

    Ptr<SpriteComponent> SongOfHeavenProjectileSprite = CreateSceneComponent<SpriteComponent>("SongOfHeavenProjectileSprite");

    SongOfHeavenProjectileSprite->AttachToComponent(ProjectileRoot);

    SongOfHeavenProjectileSprite->AddAnimationSequence("SongOfHeaven.Projectile", true);

    SongOfHeavenProjectileSprite->SetAnimationFlip(OwnerPlayer->IsRight());

    Ptr<AABBCollisionComponent> SongOfHeavenProjectileCollision = CreateSceneComponent<AABBCollisionComponent>("SongOfHeavenProjectileCollision");

    SongOfHeavenProjectileCollision->AttachToComponent(ProjectileRoot);

    SongOfHeavenProjectileCollision->SetBoxSize(96.f, 10.f);

    SongOfHeavenProjectileCollision->SetRelativePosition(OwnerPlayer->IsRight() ? 173.f : -173.f , 30.f, 0.f);
    
    SongOfHeavenProjectileCollision->SetCollisionProfile("Player");

    SongOfHeavenProjectileCollision->SetCollisionCallBack(eCollisionState::COLLISION_STATE_OVERLAP, [this, ProjectileRoot](Weak<CollisionComponent> Destination)
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

            const FVector3D HitPosition = ProjectileRoot->GetWorldPosition();

            Hit(HitPosition);
            
            ProjectileRoot->Destroy();
        });

    FSongOfHeavenProjectile SongOfHeavenProjectile;

    SongOfHeavenProjectile.Root = ProjectileRoot;

    SongOfHeavenProjectile.Collision = SongOfHeavenProjectileCollision;

    SongOfHeavenProjectile.LifeTime = 1.f;

    SongOfHeavenProjectile.CurrentLifeTime = 0.f;

    SongOfHeavenProjectile.Direction = OwnerPlayer->IsRight() ? 1.f : -1.f;

    _Projectiles.push_back(SongOfHeavenProjectile);
}

void SongOfHeaven::Hit(const FVector3D& Position)
{
    Ptr<Actor> Owner = Lock<Actor>(_Owner);
    
    if (!Owner)
    {
        return;
    }

    Ptr<SpriteComponent> SongOfHeavenHitSprite = CreateSceneComponent<SpriteComponent>("SongOfHeavenHitSprite");

    if (!SongOfHeavenHitSprite)
    {
        return;
    }

    SongOfHeavenHitSprite->AttachToComponent(GetRoot());

    SongOfHeavenHitSprite->SetWorldPosition(Position);

    SongOfHeavenHitSprite->SetWorldScale(1.f, 1.f, 1.f);

    SongOfHeavenHitSprite->AddAnimationSequence("SongOfHeaven.Hit1", false);

    _HitEffectSprites.push_back(SongOfHeavenHitSprite);
}
