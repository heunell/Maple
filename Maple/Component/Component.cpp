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
    return Lock<Actor>(_owner);
}

void Component::SetOwner(Ptr<class Actor> owner)
{
    _owner = owner;
}

Ptr<class Level> Component::GetLevel() const
{
    return Lock<Level>(_level);
}

bool Component::Init(int32 id, const std::string& name, Ptr<class Actor> owner)
{
    _id    = id;
    _name  = name;
    _owner = owner;
    _level = _level;
    
    SetEnable(true);
    SetActive(true);
    return true;
}

void Component::Tick(float deltaTime)
{}

void Component::Collision(float deltaTime)
{}

void Component::Render(float deltaTime)
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