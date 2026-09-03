#include "pch.h"
#include "SkillComponent.h"
#include "Object/Actor.h"
#include "Component/SpriteComponent.h"
#include "World/Level.h"
#include "Game/Skills/SongOfHeaven.h"
#include "Game/Skills/Anemoi/Anemoi.h"
#include "Game/Skills/VortexSphere.h"

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

    Ptr<SongOfHeaven> _SongOfHeaven = _Level->SpawnActor<SongOfHeaven>("SongOfHeaven", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if(!_SongOfHeaven)
    {
        return false;
    }
    
    Ptr<Anemoi> _Anemoi = _Level->SpawnActor<Anemoi>("Anemoi", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if(!_Anemoi)
    {
        return false;
    }
    
    _Skills[eSkillType::SongOfHeaven] = _SongOfHeaven;

    _Skills[eSkillType::Anemoi] = _Anemoi;

    return true;
}

void SkillComponent::Tick(float DeltaTime)
{
    ActorComponent::Tick(DeltaTime);
}

void SkillComponent::Destroy()
{
    for(auto& [SkillType, Skill] : _Skills)
    {
        DESTROY(Skill);
    }

    _Skills.clear();

    ActorComponent::Destroy();
}

void SkillComponent::StartSkill(eSkillType SkillType)
{
    auto FindSkill = _Skills.find(SkillType);

    if (FindSkill == _Skills.end() || !FindSkill->second)
    {
        return;
    }

    FindSkill->second->Start();
}

void SkillComponent::UseSkill(eSkillType SkillType, float DeltaTime)
{
    auto FindSkill = _Skills.find(SkillType);

    if (FindSkill == _Skills.end() || !FindSkill->second)
    {
        return;
    }

    FindSkill->second->Update(DeltaTime);
}

void SkillComponent::StopSkill(eSkillType SkillType)
{
    auto FindSkill = _Skills.find(SkillType);

    if (FindSkill == _Skills.end() || !FindSkill->second)
    {
        return;
    }

    FindSkill->second->End();
}
