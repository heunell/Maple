#include "pch.h"
#include "ActorComponent.h"

ActorComponent::ActorComponent()
{}

ActorComponent::~ActorComponent()
{}

bool ActorComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    Component::Init(Id, Name, Owner);
    _Type = COMPONENT_TYPE::ACTOR;
    return true;
}

void ActorComponent::Tick(float DeltaTime)
{
    Component::Tick(DeltaTime);
}

void ActorComponent::Destroy()
{}

void ActorComponent::Save(std::ofstream& File)
{}

void ActorComponent::Load(std::ifstream& File)
{}
