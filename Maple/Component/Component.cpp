#include "pch.h"
#include "Component.h"
#include "Object/Actor.h"
#include "World/Level.h"

Component::Component()
{}

Component::~Component()
{}

const Ptr<class Actor> Component::GetOwner() const
{
    return Lock<Actor>(_Owner);
}

void Component::SetOwner(Ptr<class Actor> owner)
{
    _Owner = owner;
}

Ptr<class Level> Component::GetLevel() const
{
    return Lock<Level>(_Level);
}

bool Component::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    _Id    = Id;
    
    _Name  = Name;
    
    _Owner = Owner;
    
    _Level = Owner->GetLevel();
    
    SetEnable(true);
    
    SetActive(true);
    
    return true;
}

void Component::Tick(float DeltaTime)
{}

void Component::Collision(float DeltaTime)
{}

void Component::Render(float DeltaTime)
{}

void Component::Destroy()
{
    SetEnable(false);
    
    SetActive(false);
}

void Component::Save(std::ofstream& file)
{}

void Component::Load(std::ifstream& file)
{}