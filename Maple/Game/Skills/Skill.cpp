#include "pch.h"
#include "Skill.h"

bool Skill::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner)
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

void Skill::Collision(float DeltaTime)
{
}

void Skill::Start(float DeltaTime)
{
}

void Skill::Update(float DeltaTime)
{
}

void Skill::End(float DeltaTime)
{
}

void Skill::Destroy()
{
    _Owner.reset();

    Actor::Destroy();
}
