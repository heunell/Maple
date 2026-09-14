#include "pch.h"
#include "SkillComponent.h"
#include "Object/Actor.h"
#include "Component/SpriteComponent.h"
#include "World/Level.h"
// Attack
#include "Game/Skills/SongOfHeaven.h"
#include "Game/Skills/Anemoi/Anemoi.h"
#include "Game/Skills/VortexSphere.h"
#include "Game/Skills/TriflingWind.h"
#include "Game/Skills/HowlingGale/HowlingGale.h"

// Buff
#include "Game/Skills/SharpEyes.h"
#include "Game/Skills/CygnusKnights.h"
#include "Game/Skills/StormBringer.h"
#include "Game/Skills/GloryOfGuardians.h"
#include "Game/Skills/DeSpell.h"

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
    
    Ptr<VortexSphere> _VortexSphere = _Level->SpawnActor<VortexSphere>("VortexSphere", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if (!_VortexSphere)
    {
        return false;
    }

    Ptr<HowlingGale> _HowlingGale = _Level->SpawnActor<HowlingGale>("HowlingGale", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if (!_HowlingGale)
    {
        return false;
    }

    Ptr<SharpEyes> _SharpEyes = _Level->SpawnActor<SharpEyes>("SharpEyes", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if (!_SharpEyes)
    {
        return false;
    }

    Ptr<CygnusKnights> _CygnusKnights = _Level->SpawnActor<CygnusKnights>("CygnusKnights", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if (!_CygnusKnights)
    {
        return false;
    }

    Ptr<StormBringer> _StormBringer = _Level->SpawnActor<StormBringer>("StormBringer", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if (!_StormBringer)
    {
        return false;
    }

    Ptr<GloryOfGuardians> _GloryOfGuardians = _Level->SpawnActor<GloryOfGuardians>("GloryOfGuardians", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if (!_GloryOfGuardians)
    {
        return false;
    }

    Ptr<DeSpell> _DeSpell = _Level->SpawnActor<DeSpell>("DeSpell", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if (!_DeSpell)
    {
        return false;
    }

    Ptr<TriflingWind> _TriflingWind = _Level->SpawnActor<TriflingWind>("TriflingWind", FVector3D::Zero, FVector3D(1.f, 1.f, 1.f), FRotator(0.f, 0.f, 0.f), Owner);

    if (!_TriflingWind)
    {
        return false;
    }

    _Skills[eSkillType::TriflingWind] = _TriflingWind;

    _Skills[eSkillType::SongOfHeaven] = _SongOfHeaven;

    _Skills[eSkillType::Anemoi]       = _Anemoi;

    _Skills[eSkillType::VortexSphere] = _VortexSphere;

    _Skills[eSkillType::HowlingGale]  = _HowlingGale;

    _Skills[eSkillType::SharpEyes]    = _SharpEyes;

    _Skills[eSkillType::CKnight]      = _CygnusKnights;

    _Skills[eSkillType::StormBringer] = _StormBringer;

    _Skills[eSkillType::GloryOfGuardians] = _GloryOfGuardians;

    _Skills[eSkillType::DeSpell]      = _DeSpell;

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

bool SkillComponent::IsSkillActive(eSkillType SkillType) const
{
    auto FindSkill = _Skills.find(SkillType);

    if (FindSkill == _Skills.end() || !FindSkill->second)
    {
        return false;
    }

    return FindSkill->second->IsActive();
}
