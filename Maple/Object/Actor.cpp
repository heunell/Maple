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

bool Actor::Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name)
{
    _id   = id;
    _name = name;
    _type = eActorType::Actor;
    _root = CreateSceneComponent<SceneComponent>("DefaultRoot");
    SetRelativePosition(pos);
    SetRelativeScale(scale);
    SetRelativeRotation(rot);
    SetEnable(true);
    SetActive(true);
    return true;
}

void Actor::Tick(float deltaTime)
{
    _root->Tick(deltaTime);
    for (auto& it : _actorComponents)
    {
        Ptr<ActorComponent> comp = it.second;
        if (nullptr == comp)
            continue;
        if (!comp->IsActive() || !comp->IsEnable())
            continue;
        comp->Tick(deltaTime);
    }
}

void Actor::Collision(float deltaTime)
{}

void Actor::Render(float deltaTime)
{
    // _root->Render(deltaTime);
}

void Actor::Destroy()
{
}

Ptr<class SceneComponent> Actor::GetRoot() const
{
    return _root;
}

void Actor::SetRootComponent(const Ptr<class SceneComponent> comp)
{
    if (_root)
    {
        comp->SetRelativeTransform(_root->GetRelativeTransform());
        for (auto& it : _root->_childs)
        {
            Ptr<SceneComponent> child = it.second;
            if (child)
            {
                child->AttachToComponent(comp);
            }
        }
        _root->_childs.clear();
        Delete(_root);
        _root = comp;
    }
    else
    {
        _root = comp;
    }
}

Ptr<class SceneComponent> Actor::FindSceneComponent(const std::string& name) const
{
    if (_root->GetName() == name)
        return _root;
    auto it = _componentFinder.find(name);
    if (_componentFinder.end() == it)
        return nullptr;
    return FindSceneComponent(it->second);
}

Ptr<class ActorComponent> Actor::FindActorComponent(const std::string& name) const
{   
    auto it = _componentFinder.find(name);
    if (_componentFinder.end() == it)
        return nullptr;
    return FindActorComponent(it->second);
}

Ptr<class SceneComponent> Actor::FindSceneComponent(int32 id) const
{
    if (_root->GetComponentID() == id)
        return _root;
    return _root->FindComponent(id);
}

Ptr<class ActorComponent> Actor::FindActorComponent(int32 id) const
{
    auto compIt = _actorComponents.find(id);
    if (_actorComponents.end() == compIt)
        return nullptr;
    return compIt->second;
}

Ptr<class Level> Actor::GetLevel() const
{
    return Lock<Level>(_level);
}

void Actor::SetLevel(Ptr<class Level> level)
{
    _level = level;
}

void Actor::Remove()
{
    Ptr<Level> level = Lock<Level>(_level);
    if (level)
        level->RemoveActor(_id);
}

void Actor::SetName(const std::string& name)
{
    _name = name;
}

const std::string& Actor::GetName() const
{
    return _name;
}

eActorType Actor::GetType() const
{
    return _type;
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
    level->AddTag(tag, _id);
}

const FTransform& Actor::GetWorldTransform() const
{
    return _root->GetWorldTransform();
}

void Actor::SetWorldTransform(const FTransform& transform)
{
}

void Actor::SetWorldTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
}

const FVector3D& Actor::GetWorldScale() const
{
}

void Actor::SetWorldScale(const FVector3D& scale)
{
}

void Actor::SetWorldScale(float x, float y, float z)
{
}

void Actor::SetWorldScale(const FVector2D& scale)
{
}

void Actor::SetWorldScale(float x, float y)
{
}

const FVector3D& Actor::AddWorldScale(const FVector3D& scale)
{
}

const FVector3D& Actor::AddWorldScale(float x, float y, float z)
{
}

const FVector3D& Actor::AddWorldScale(const FVector2D& scale)
{
}

const FVector3D& Actor::AddWorldScale(float x, float y)
{
}

const FVector3D& Actor::AddWorldScale(float value)
{
}

const FVector3D& Actor::AddWorldScaleX(float x)
{
}

const FVector3D& Actor::AddWorldScaleY(float y)
{
}

const FVector3D& Actor::GetWorldPosition() const
{
}

void Actor::SetWorldPosition(const FVector3D& pos)
{
}

void Actor::SetWorldPosition(float x, float y, float z)
{
}

void Actor::SetWorldPosition(const FVector2D& pos)
{
}

void Actor::SetWorldPosition(float x, float y)
{
}

const FVector3D& Actor::AddWorldPosition(const FVector3D& pos)
{
}

const FVector3D& Actor::AddWorldPosition(float x, float y, float z)
{
}

const FVector3D& Actor::AddWorldPosition(const FVector2D& pos)
{
}

const FVector3D& Actor::AddWorldPosition(float x, float y)
{
}

const FVector3D& Actor::AddWorldPosition(float value)
{
}

const FVector3D& Actor::AddWorldPositionX(float x)
{
}

const FVector3D& Actor::AddWorldPositionY(float y)
{
}

const FRotator& Actor::GetWorldRotation() const
{
}

void Actor::SetWorldRotation(const FRotator& rot)
{
}

void Actor::SetWorldRotation(float x, float y, float z)
{
}

void Actor::SetWorldRotation(const FVector2D& rot)
{
}

void Actor::SetWorldRotation(float x, float y)
{
}

const FTransform& Actor::GetRelativeTransform() const
{
}

void Actor::SetRelativeTransform(const FTransform& transform)
{
}

void Actor::SetRelativeTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
{
}

const FVector3D& Actor::GetRelativeScale() const
{
}

void Actor::SetRelativeScale(const FVector3D& scale)
{
}

void Actor::SetRelativeScale(float x, float y, float z)
{
}

void Actor::SetRelativeScale(const FVector2D& scale)
{
}

void Actor::SetRelativeScale(float x, float y)
{
}

const FVector3D& Actor::AddRelativeScale(const FVector3D& scale)
{
}

const FVector3D& Actor::AddRelativeScale(float x, float y, float z)
{
}

const FVector3D& Actor::AddRelativeScale(const FVector2D& scale)
{
}

const FVector3D& Actor::AddRelativeScale(float x, float y)
{
}

const FVector3D& Actor::AddRelativeScale(float value)
{
}

const FVector3D& Actor::AddRelativeScaleX(float x)
{
}

const FVector3D& Actor::AddRelativeScaleY(float y)
{
}

const FVector3D& Actor::GetRelativePosition() const
{
}

void Actor::SetRelativePosition(const FVector3D& pos)
{
}

void Actor::SetRelativePosition(float x, float y, float z)
{
}

void Actor::SetRelativePosition(const FVector2D& pos)
{
}

void Actor::SetRelativePosition(float x, float y)
{
}

const FVector3D& Actor::AddRelativePosition(const FVector3D& scale)
{
}

const FVector3D& Actor::AddRelativePosition(float x, float y, float z)
{
}

const FVector3D& Actor::AddRelativePosition(const FVector2D& scale)
{
}

const FVector3D& Actor::AddRelativePosition(float x, float y)
{
}

const FVector3D& Actor::AddRelativePosition(float value)
{
}

const FVector3D& Actor::AddRelativePositionX(float x)
{
}

const FVector3D& Actor::AddRelativePositionY(float y)
{
}

const FRotator& Actor::GetRelativeRotation() const
{
}

void Actor::SetRelativeRotation(const FRotator& rot)
{
}

void Actor::SetRelativeRotation(float x, float y, float z)
{
}

void Actor::SetRelativeRotation(const FVector2D& rot)
{
}

void Actor::SetRelativeRotation(float x, float y)
{
}

const FRotator& Actor::AddRelativeRotation(const FVector3D& scale)
{
}

const FRotator& Actor::AddRelativeRotation(float x, float y, float z)
{
}

const FRotator& Actor::AddRelativeRotation(const FVector2D& scale)
{
}

const FRotator& Actor::AddRelativeRotation(float x, float y)
{
}

const FRotator& Actor::AddRelativeRotation(float value)
{
}

const FRotator& Actor::AddRelativeRotationX(float x)
{
}

const FRotator& Actor::AddRelativeRotationY(float y)
{
}
