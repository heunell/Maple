#include "pch.h"
#include "Anemoi.h"

#include "AnemoiActor.h"
#include "Component/SpriteComponent.h"
#include "Game/Character/Player.h"
#include "World/Level.h"

bool Anemoi::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner)
{
    if (!Skill::Init(Id, Position, Scale, Rotator, Name, Owner))
    {
        return false;
    }

    Ptr<SpriteComponent> SkillSprite = Owner->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (!SkillSprite)
    {
        return false;
    }

    _CoolDown = _Datas.Cooldown;

    SkillSprite->AddAnimationSequence("Anemoi.Cast", false);

    return true;
}

void Anemoi::Tick(float DeltaTime)
{
    Skill::Tick(DeltaTime);

    if (!_Casting)
    {
        return;
    }

    Ptr<Actor> Owner = Lock(_Owner);

    if (!Owner)
    {
        _Casting = false;

        return;
    }

    Ptr<Player> OwnerPlayer = Cast<Actor, Player>(Owner);

    Ptr<SpriteComponent> SkillSprite = Owner->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (!OwnerPlayer || !SkillSprite)
    {
        _Casting = false;

        return;
    }

    Ptr<Animation2D> Animation = SkillSprite->GetAnimation();

    if (!Animation || !Animation->IsFinished())
    {
        return;
    }

    Ptr<Level> Level = OwnerPlayer->GetLevel();

    if (Level)
    {
        const FVector3D SpawnPosition = OwnerPlayer->GetWorldPosition() + (OwnerPlayer->IsRight() ? _Datas.TraceRightPosition : _Datas.TraceLeftPosition);

        Level->SpawnActor<AnemoiActor>("AnemoiActor", SpawnPosition, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), OwnerPlayer, _Datas);
    }

    SkillSprite->SetEnable(false);

    _Casting = false;
}

void Anemoi::Collision(float DeltaTime)
{
}

void Anemoi::Start()
{
    if (_Casting || !CanUse())
    {
        return;
    }

    Ptr<Actor> Owner = Lock(_Owner);

    if (!Owner)
    {
        return;
    }

    Ptr<Player> OwnerPlayer = Cast<Actor, Player>(Owner);

    Ptr<SpriteComponent> SkillSprite = Owner->FindSceneComponent<SpriteComponent>("SkillSprite");

    if (!OwnerPlayer || !SkillSprite)
    {
        return;
    }

    SkillSprite->SetAnimationFlip(OwnerPlayer->IsRight());

    SkillSprite->SetEnable(true);

    SkillSprite->ChangeAnimation("Anemoi.Cast");

    _Casting = true;

    StartCoolDown();
}

void Anemoi::Update(float DeltaTime)
{
}

void Anemoi::End()
{
}

bool Anemoi::IsActive() const
{
    return _Casting;
}

void Anemoi::Destroy()
{
    _Casting = false;

    Skill::Destroy();
}