#include "pch.h"
#include "Skill.h"
#include "Game/Character/Player.h"

bool Skill::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name, Ptr<Actor> Owner)
{
    if (!Owner)
    {
        return false;
    }

    _Owner = Owner;

    if (!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    return true;
}

void Skill::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);

    if(_RemainCoolDown <= 0.f)
    {
        return;
    }

    _RemainCoolDown -= DeltaTime;

    if(_RemainCoolDown < 0.f)
    {
        _RemainCoolDown = 0.f;
    }
}

void Skill::Collision(float DeltaTime)
{}

void Skill::Start()
{}

void Skill::Update(float DeltaTime)
{}

void Skill::End()
{}

void Skill::Destroy()
{
    _Owner.reset();

    Actor::Destroy();
}

bool Skill::IsActive() const
{
    return false;
}

bool Skill::CanUse() const
{
    if (_RemainCoolDown > 0.f)
    {
        return false;
    }
    
    Ptr<Player> PlayerOwner = Cast<Actor, Player>(Lock(_Owner));
    
    return PlayerOwner && PlayerOwner->CanUseMP(_MPCost);
}

void Skill::StartCoolDown()
{
    Ptr<Player> PlayerOwner = Cast<Actor, Player>(Lock(_Owner));
    
    if (!PlayerOwner || !PlayerOwner->UseMP(_MPCost))
    {
        return;
    }
    
    _RemainCoolDown = _CoolDown;
}
