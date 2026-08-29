#include "pch.h"
#include "SkillComponent.h"
#include "Object/Actor.h"
#include "Component/SpriteComponent.h"
#include "World/Level.h"
#include "Game/Skills/SongOfHeaven.h"

SkillComponent::SkillComponent()
{}

SkillComponent::~SkillComponent()
{}

bool SkillComponent::Init(int32 Id, const std::string& Name, Ptr<Actor> Owner)
{
    if (!ActorComponent::Init(Id, Name, Owner))
    {
        return false;
    }

    Ptr<Level> _Level = Owner->GetLevel();

    if (!_Level)
    {
        return false;
    }

    _SongOfHeaven = _Level->SpawnActor<SongOfHeaven>("SongOfHeaven", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    return _SongOfHeaven != nullptr;
}

void SkillComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);
}

void SkillComponent::Destroy()
{
    DESTROY(_SongOfHeaven);

    ActorComponent::Destroy();
}

void SkillComponent::StartSkill()
{
    if (_SongOfHeaven)
    {
        _SongOfHeaven->Start();
    }
}

void SkillComponent::UseSkill(float DeltaTime)
{
    if (_SongOfHeaven)
    {
        _SongOfHeaven->Update(DeltaTime);
    }
}

void SkillComponent::StopSkill()
{
    if (_SongOfHeaven)
    {
        _SongOfHeaven->End();
    }
}
