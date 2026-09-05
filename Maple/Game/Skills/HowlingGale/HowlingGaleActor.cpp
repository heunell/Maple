#include "pch.h"
#include "HowlingGaleActor.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"

HowlingGaleActor::HowlingGaleActor()
{}

HowlingGaleActor::~HowlingGaleActor()
{}

bool HowlingGaleActor::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<Player> Owner, const FHowlingGaleDatas& Datas, int32 Stack, float Direction)
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

    _Stack = Stack >= 2 ? 2 : 1;

    _Direction = Direction >= 0.f ? 1.f : -1.f;

    AddTag("PlayerAttack");

    _Sprite = CreateSceneComponent<SpriteComponent>("HowlingGaleSprite");

    if (!_Sprite)
    {
        return false;
    }

    _Sprite->AttachToComponent(GetRoot());

    _Sprite->SetRenderLayerName("SkillFront");

    _Sprite->AddAnimationSequence("HowlingGale.Stack1.Projectile", true);

    _Sprite->AddAnimationSequence("HowlingGale.Stack1.Die", false);

    _Sprite->AddAnimationSequence("HowlingGale.Stack2.Projectile", true);

    _Sprite->AddAnimationSequence("HowlingGale.Stack2.Die", false);

    _Sprite->SetAnimationFlip(_Direction > 0.f);

    if (_Stack == 1)
    {
        _Sprite->ChangeAnimation("HowlingGale.Stack1.Projectile");
    }
    else
    {
        _Sprite->ChangeAnimation("HowlingGale.Stack2.Projectile");
    }

    _AttackRange = CreateSceneComponent<AABBCollisionComponent>("HowlingGaleAttackRange");

    if (!_AttackRange)
    {
        return false;
    }

    _AttackRange->AttachToComponent(GetRoot());

    if (_Stack == 1)
    {
        _AttackRange->SetBoxSize(_Datas.Stack1RangeWidth, _Datas.Stack1RangeHeight);
    }
    else
    {
        _AttackRange->SetBoxSize(_Datas.Stack2RangeWidth, _Datas.Stack2RangeHeight);
    }

    _AttackRange->SetCollisionProfile("PlayerAttack");

    _AttackRange->SetCollisionCallBack(eCollisionState::COLLISION_STATE_OVERLAP, this, &HowlingGaleActor::OnMonsterOverlap);

    _CurrentLifeTime = 0.f;

    _HitTimer = _Datas.HitInterval;

    return true;
}

void HowlingGaleActor::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);

    if (_IsEnding)
    {
        if (!_Sprite)
        {
            Remove();

            return;
        }

        Ptr<Animation2D> Animation = _Sprite->GetAnimation();

        if (Animation && Animation->IsFinished())
        {
            Remove();
        }

        return;
    }

    _CurrentLifeTime += DeltaTime;

    if (_CurrentLifeTime >= _Datas.LifeTime)
    {
        BeginEnd();

        return;
    }

    float MoveSpeed = _IsMonsterOverlap ? _Datas.HitMoveSpeed : _Datas.MoveSpeed;

    GetRoot()->AddWorldPositionX(_Direction * MoveSpeed * DeltaTime);

    if (_IsMonsterOverlap)
    {
        _HitTimer += DeltaTime;

        if (_HitTimer >= _Datas.HitInterval)
        {
            Hit();

            _HitTimer -= _Datas.HitInterval;
        }
    }
    else
    {
        _HitTimer = _Datas.HitInterval;
    }

    _IsMonsterOverlap = false;

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

void HowlingGaleActor::Destroy()
{
    _Owner.reset();

    _Sprite.reset();

    _AttackRange.reset();

    _HitSprites.clear();

    Actor::Destroy();
}

void HowlingGaleActor::OnMonsterOverlap(Weak<CollisionComponent> Destination)
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

    _HitPosition = Monster->GetWorldPosition();

    _IsMonsterOverlap = true;
}

void HowlingGaleActor::Hit()
{
    Ptr<SpriteComponent> HitSprite = CreateSceneComponent<SpriteComponent>("HowlingGaleHitSprite");

    if (!HitSprite)
    {
        return;
    }

    HitSprite->AttachToComponent(GetRoot());

    HitSprite->SetWorldPosition(_HitPosition);

    HitSprite->SetRenderLayerName("SkillFront");

    if (_Stack == 1)
    {
        HitSprite->AddAnimationSequence("HowlingGale.Stack1.Hit", false);

        HitSprite->ChangeAnimation("HowlingGale.Stack1.Hit");
    }
    else
    {
        HitSprite->AddAnimationSequence("HowlingGale.Stack2.Hit", false);

        HitSprite->ChangeAnimation("HowlingGale.Stack2.Hit");
    }

    _HitSprites.push_back(HitSprite);
}

void HowlingGaleActor::BeginEnd()
{
    if (_IsEnding)
    {
        return;
    }

    _IsEnding = true;

    if (_AttackRange)
    {
        _AttackRange->SetEnable(false);
    }

    if (!_Sprite)
    {
        Remove();

        return;
    }

    if (_Stack == 1)
    {
        _Sprite->ChangeAnimation("HowlingGale.Stack1.Die");

        _Sprite->SetAnimationFrame(0);

        _Sprite->SetPlay("HowlingGale.Stack1.Die", true);
    }
    else
    {
        _Sprite->ChangeAnimation("HowlingGale.Stack2.Die");

        _Sprite->SetAnimationFrame(0);

        _Sprite->SetPlay("HowlingGale.Stack2.Die", true);
    }
}