#include "pch.h"
#include "HowlingGale.h"
#include "HowlingGaleActor.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"
#include "World/Level.h"

HowlingGale::HowlingGale()
{}

HowlingGale::~HowlingGale()
{}

bool HowlingGale::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner)
{
    if (!Skill::Init(Id, Position, Scale, Rotator, Name, Owner))
    {
        return false;
    }

    _CastSprite = CreateSceneComponent<SpriteComponent>("HowlingGaleCastSprite");

    if (!_CastSprite)
    {
        return false;
    }

    _CastSprite->AttachToComponent(GetRoot());

    _CastSprite->SetRenderLayerName("SkillFront");

    _CastSprite->AddAnimationSequence("HowlingGale.Cast", false);

    _CastSprite->AddNotify("HowlingGale.Cast", 6, this, &HowlingGale::Fire);

    _CastSprite->SetEnable(false);

    _CoolDown = _Datas.Cooldown;

    return true;
}

void HowlingGale::Tick(float DeltaTime)
{
    Skill::Tick(DeltaTime);

    if (!_IsCasting)
    {
        return;
    }

    Ptr<Actor> Owner = Lock<Actor>(_Owner);

    if (!Owner || !_CastSprite)
    {
        FinishCast();

        return;
    }

    _CastSprite->SetWorldPosition(Owner->GetWorldPosition());

    Ptr<Animation2D> Animation = _CastSprite->GetAnimation();

    if (Animation && Animation->IsFinished())
    {
        FinishCast();
    }
}

void HowlingGale::Collision(float DeltaTime)
{
}

void HowlingGale::Start()
{
    if (_IsCharging || _IsCasting || !CanUse())
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

    _Direction = OwnerPlayer->IsRight() ? 1.f : -1.f;

    _CurrentChargeTime = 0.f;

    _Stack = 1;

    _IsCharging = true;
}

void HowlingGale::Update(float DeltaTime)
{
    if (!_IsCharging)
    {
        return;
    }

    _CurrentChargeTime += DeltaTime;

    if (_CurrentChargeTime >= _Datas.ChargeTime)
    {
        _Stack = 2;
    }
}

void HowlingGale::End()
{
    if (!_IsCharging)
    {
        return;
    }

    Ptr<Actor> Owner = Lock<Actor>(_Owner);

    if (!Owner || !_CastSprite)
    {
        _IsCharging = false;

        return;
    }

    Ptr<Player> OwnerPlayer = Cast<Actor, Player>(Owner);

    if (!OwnerPlayer)
    {
        _IsCharging = false;

        return;
    }

    _IsCharging = false;

    _IsCasting = true;

    _CastSprite->SetWorldPosition(Owner->GetWorldPosition());

    _CastSprite->SetAnimationFlip(OwnerPlayer->IsRight());

    _CastSprite->SetEnable(true);

    _CastSprite->ChangeAnimation("HowlingGale.Cast");

    _CastSprite->SetAnimationFrame(0);

    _CastSprite->SetPlay("HowlingGale.Cast", true);

    StartCoolDown();
}

void HowlingGale::Destroy()
{
    _IsCharging = false;

    _IsCasting = false;

    DESTROY(_CastSprite);

    Skill::Destroy();
}

bool HowlingGale::IsActive() const
{
    return _IsCharging || _IsCasting;
}

void HowlingGale::Fire()
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

    Ptr<Level> Level = OwnerPlayer->GetLevel();

    if (!Level)
    {
        return;
    }

    FVector3D SpawnPosition = OwnerPlayer->GetWorldPosition();

    SpawnPosition += _Direction > 0.f ? _Datas.RightPosition : _Datas.LeftPosition;

    Level->SpawnActor<HowlingGaleActor>("HowlingGaleActor", SpawnPosition, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), OwnerPlayer, _Datas, _Stack, _Direction);
}

void HowlingGale::FinishCast()
{
    _IsCasting = false;

    if (_CastSprite)
    {
        _CastSprite->SetEnable(false);
    }
}