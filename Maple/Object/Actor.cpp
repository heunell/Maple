#include "pch.h"
#include "Actor.h"
#include "World/Level.h"
#include "Component/SceneComponent.h"
#include "Component/ActorComponent.h"
#include <filesystem>
#include <fstream>

Actor::Actor()
{}

Actor::~Actor()
{}

bool Actor::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotatar, const std::string& Name)
{
    _Id   = Id;
    _Name = Name;
    _Type = eActorType::Actor;
    _Root = CreateSceneComponent<SceneComponent>("DefaultRoot");
    SetRelativePosition(Position);
    SetRelativeScale(Scale);
    SetRelativeRotation(Rotatar);
    SetEnable(true);
    SetActive(true);
    return true;
}

void Actor::Tick(float DeltaTime)
{
    _Root->Tick(DeltaTime);
    for (auto& it : _ActorComponents)
    {
        Ptr<ActorComponent> comp = it.second;
        if (nullptr == comp)
            continue;
        if (!comp->IsActive() || !comp->IsEnable())
            continue;
        comp->Tick(DeltaTime);
    }
}

void Actor::Collision(float DeltaTime)
{}

void Actor::Render(float DeltaTime)
{
    // _Root->Render(deltaTime);
}

void Actor::Destroy()
{
}

void Actor::Save(std::ofstream& File)
{
}

void Actor::Load(std::ifstream& File)
{
}

Ptr<class SceneComponent> Actor::GetRoot() const
{
    return _Root;
}

void Actor::SetRootComponent(const Ptr<class SceneComponent> comp)
{
    if (_Root)
    {
        comp->SetRelativeTransform(_Root->GetRelativeTransform());
        for (auto& it : _Root->_Childs)
        {
            Ptr<SceneComponent> child = it.second;
            if (child)
            {
                child->AttachToComponent(comp);
            }
        }
        _Root->_Childs.clear();
        Delete(_Root);
        _Root = comp;
    }
    else
    {
        _Root = comp;
    }
}

Ptr<class SceneComponent> Actor::FindSceneComponent(const std::string& Name) const
{
    if (_Root->GetName() == Name)
        return _Root;
    auto it = _ComponentFinder.find(Name);
    if (_ComponentFinder.end() == it)
        return nullptr;
    return FindSceneComponent(it->second);
}

Ptr<class ActorComponent> Actor::FindActorComponent(const std::string& Name) const
{   
    auto it = _ComponentFinder.find(Name);
    if (_ComponentFinder.end() == it)
        return nullptr;
    return FindActorComponent(it->second);
}

Ptr<class SceneComponent> Actor::FindSceneComponent(int32 Id) const
{
    if (_Root->GetComponentID() == Id)
        return _Root;
    return _Root->FindComponent(Id);
}

Ptr<class ActorComponent> Actor::FindActorComponent(int32 Id) const
{
    auto compIt = _ActorComponents.find(Id);
    if (_ActorComponents.end() == compIt)
        return nullptr;
    return compIt->second;
}

Ptr<class Level> Actor::GetLevel() const
{
    return Lock<Level>(_Level);
}

void Actor::SetLevel(Ptr<class Level> level)
{
    _Level = level;
}

void Actor::Remove()
{
    Ptr<Level> level = Lock<Level>(_Level);
    if (level)
        level->RemoveActor(_Id);
}

void Actor::SetName(const std::string& Name)
{
    _Name = Name;
}

const std::string& Actor::GetName() const
{
    return _Name;
}

eActorType Actor::GetType() const
{
    return _Type;
}

bool Actor::IsTag(const std::string& tag)
{
    return _tags.contains(tag);
}

void Actor::AddTag(const std::string& tag)
{
    _tags.insert(tag);
    Ptr<Level> level = GetLevel();
    if (nullptr == level)
        return;
    level->AddTag(tag, _Id);
}

const FTransform& Actor::GetWorldTransform() const
{
    return _Root->GetWorldTransform();
}

void Actor::SetWorldTransform(const FTransform& trans)
{
    _Root->SetWorldTransform(trans);
}

void Actor::SetWorldTransform(const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator)
{
    _Root->SetWorldTransform(Position, Scale, Rotator);
}

const FVector3D& Actor::GetWorldScale() const
{
    return _Root->GetWorldScale();
}

void Actor::SetWorldScale(const FVector3D& Scale)
{
    _Root->SetWorldScale(Scale);
}

void Actor::SetWorldScale(float x, float y, float z)
{
    _Root->SetWorldScale(x, y, z);
}

void Actor::SetWorldScale(const FVector2D& Scale)
{
    _Root->SetWorldScale(Scale);
}

void Actor::SetWorldScale(float x, float y)
{
    _Root->SetWorldScale(x, y);
}

const FVector3D& Actor::AddWorldScale(const FVector3D& Scale)
{
    return _Root->AddWorldScale(Scale);
}

const FVector3D& Actor::AddWorldScale(float x, float y, float z)
{
    return _Root->AddWorldScale(x, y, z);
}

const FVector3D& Actor::AddWorldScale(const FVector2D& Scale)
{
    return _Root->AddWorldScale(Scale);
}

const FVector3D& Actor::AddWorldScale(float x, float y)
{
    return _Root->AddWorldScale(x, y);
}

const FVector3D& Actor::AddWorldScale(float value)
{
    return _Root->AddWorldScale(value);
}

const FVector3D& Actor::AddWorldScaleX(float x)
{
    return _Root->AddWorldScale(x);
}

const FVector3D& Actor::AddWorldScaleY(float y)
{
    return _Root->AddWorldScale(y);
}

const FVector3D& Actor::GetWorldPosition() const
{
    return _Root->GetWorldPosition();
}

void Actor::SetWorldPosition(const FVector3D& Position)
{
    _Root->SetWorldPosition(Position);
}

void Actor::SetWorldPosition(float x, float y, float z)
{
    _Root->SetWorldPosition(x, y, z);
}

void Actor::SetWorldPosition(const FVector2D& Position)
{
    _Root->SetWorldPosition(Position);
}

void Actor::SetWorldPosition(float x, float y)
{
    _Root->SetWorldPosition(x, y);
}

const FVector3D& Actor::AddWorldPosition(const FVector3D& Position)
{
    return _Root->AddWorldPosition(Position);
}

const FVector3D& Actor::AddWorldPosition(float x, float y, float z)
{
    return _Root->AddWorldPosition(x, y, z);
}

const FVector3D& Actor::AddWorldPosition(const FVector2D& Position)
{
    return _Root->AddWorldPosition(Position);
}

const FVector3D& Actor::AddWorldPosition(float x, float y)
{
    return _Root->AddWorldPosition(x, y);
}

const FVector3D& Actor::AddWorldPosition(float value)
{
    return _Root->AddWorldPosition(value);
}

const FVector3D& Actor::AddWorldPositionX(float x)
{
    return _Root->AddWorldPosition(x);
}

const FVector3D& Actor::AddWorldPositionY(float y)
{
    return _Root->AddWorldPosition(y);
}

const FRotator& Actor::GetWorldRotation() const
{
    return _Root->GetWorldRotation();
}

void Actor::SetWorldRotation(const FRotator& Rotator)
{
    _Root->SetWorldRotation(Rotator);
}

void Actor::SetWorldRotation(float x, float y, float z)
{
    _Root->SetWorldRotation(x, y, z);
}

void Actor::SetWorldRotation(const FVector2D& Rotator)
{
    _Root->SetWorldRotation(Rotator);
}

void Actor::SetWorldRotation(float x, float y)
{
    _Root->SetWorldRotation(x, y);
}

const FTransform& Actor::GetRelativeTransform() const
{
    return _Root->GetRelativeTransform();
}

void Actor::SetRelativeTransform(const FTransform& trans)
{
    _Root->SetRelativeTransform(trans);
}

void Actor::SetRelativeTransform(const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator)
{
    _Root->SetRelativeTransform(Position, Scale, Rotator);
}

const FVector3D& Actor::GetRelativeScale() const
{
    return _Root->GetRelativeScale();
}

void Actor::SetRelativeScale(const FVector3D& Scale)
{
    _Root->SetRelativeScale(Scale);
}

void Actor::SetRelativeScale(float x, float y, float z)
{
    _Root->SetRelativeScale(x, y, z);
}

void Actor::SetRelativeScale(const FVector2D& Scale)
{
    _Root->SetRelativeScale(Scale);
}

void Actor::SetRelativeScale(float x, float y)
{
    _Root->SetRelativeScale(x, y);
}

const FVector3D& Actor::AddRelativeScale(const FVector3D& Scale)
{
    return _Root->AddRelativeScale(Scale);
}

const FVector3D& Actor::AddRelativeScale(float x, float y, float z)
{
    return _Root->AddRelativeScale(x, y, z);
}

const FVector3D& Actor::AddRelativeScale(const FVector2D& Scale)
{
    return _Root->AddRelativeScale(Scale);
}

const FVector3D& Actor::AddRelativeScale(float x, float y)
{
    return _Root->AddRelativeScale(x, y);
}

const FVector3D& Actor::AddRelativeScale(float value)
{
    return _Root->AddRelativeScale(value);
}

const FVector3D& Actor::AddRelativeScaleX(float x)
{
    return _Root->AddRelativeScaleX(x);
}

const FVector3D& Actor::AddRelativeScaleY(float y)
{
    return _Root->AddRelativeScaleY(y);
}

const FVector3D& Actor::GetRelativePosition() const
{
    return _Root->GetRelativePosition();
}

void Actor::SetRelativePosition(const FVector3D& Position)
{
    _Root->SetRelativePosition(Position);
}

void Actor::SetRelativePosition(float x, float y, float z)
{
    _Root->SetRelativePosition(x, y, z);
}

void Actor::SetRelativePosition(const FVector2D& Position)
{
    _Root->SetRelativePosition(Position);
}

void Actor::SetRelativePosition(float x, float y)
{
    _Root->SetRelativePosition(x, y);
}

const FVector3D& Actor::AddRelativePosition(const FVector3D& Scale)
{
    return _Root->AddRelativePosition(Scale);
}

const FVector3D& Actor::AddRelativePosition(float x, float y, float z)
{
    return _Root->AddRelativePosition(x, y, z);
}

const FVector3D& Actor::AddRelativePosition(const FVector2D& Scale)
{
    return _Root->AddRelativePosition(Scale);
}

const FVector3D& Actor::AddRelativePosition(float x, float y)
{
    return _Root->AddRelativePosition(x, y);
}

const FVector3D& Actor::AddRelativePosition(float value)
{
    return _Root->AddRelativePosition(value);
}

const FVector3D& Actor::AddRelativePositionX(float x)
{
    return _Root->AddRelativePositionX(x);
}

const FVector3D& Actor::AddRelativePositionY(float y)
{
    return _Root->AddRelativePositionY(y);
}

const FRotator& Actor::GetRelativeRotation() const
{
    return _Root->GetRelativeRotation();
}

void Actor::SetRelativeRotation(const FRotator& Rotator)
{
    _Root->SetRelativeRotation(Rotator);
}

void Actor::SetRelativeRotation(float x, float y, float z)
{
    _Root->SetRelativeRotation(x, y, z);
}

void Actor::SetRelativeRotation(const FVector2D& Rotator)
{
    _Root->SetRelativeRotation(Rotator);
}

void Actor::SetRelativeRotation(float x, float y)
{
    _Root->SetRelativeRotation(x, y);
}

const FRotator& Actor::AddRelativeRotation(const FVector3D& Scale)
{
    return _Root->AddRelativeRotation(Scale);
}

const FRotator& Actor::AddRelativeRotation(float x, float y, float z)
{
    return _Root->AddRelativeRotation(x, y, z);
}

const FRotator& Actor::AddRelativeRotation(const FVector2D& Scale)
{
    return _Root->AddRelativeRotation(Scale);
}

const FRotator& Actor::AddRelativeRotation(float x, float y)
{
    return _Root->AddRelativeRotation(x, y);
}

const FRotator& Actor::AddRelativeRotation(float value)
{
    return _Root->AddRelativeRotation(value);
}

const FRotator& Actor::AddRelativeRotationX(float x)
{
    return _Root->AddRelativeRotationX(x);
}

const FRotator& Actor::AddRelativeRotationY(float y)
{
    return _Root->AddRelativeRotationY(y);
}
