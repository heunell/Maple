#pragma once
#include "Core/Object.h"
#include "Common/IArchive.h"
#include <map>
#include <unordered_map>
#include <set>

class Actor : public Object, public IArchive
{
    friend class Level;
public:
    Actor();
    virtual ~Actor();
    
protected:
    std::string _name;
    int32 _componentID = 0;
    int32 _id = -1;
    std::set<std::string> _tags;
    eActorType _type = eActorType::Actor;
    
protected:
    Ptr<class SceneComponent> _root;
    Weak<class Level> _level;
    std::map<int, Ptr<class ActorComponent>> _actorComponents;
    std::unordered_map<std::string, int> _componentFinder;
    
public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name);
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;
    
public:
    Ptr<class SceneComponent> GetRoot() const;
    void SetRootComponent(const Ptr<class SceneComponent> comp);
    Ptr<class SceneComponent> FindSceneComponent(const std::string& name) const;
    Ptr<class ActorComponent> FindActorComponent(const std::string& name) const;
    Ptr<class SceneComponent> FindSceneComponent(int32 id) const;
    Ptr<class ActorComponent> FindActorComponent(int32 id) const;
    Ptr<class Level> GetLevel() const;
    void SetLevel(Ptr<class Level> level);
    
public:
    void Remove();
    void SetName(const std::string& name);
    const std::string& GetName() const;
    
    eActorType GetType() const;
    
public:
    const int32 GetActorID() const { return _id; }
    bool IsTag(const std::string& tag);
    void AddTag(const std::string& tag);
    
public:
    template<typename ...Args>
    void AddTags(Args&& ... args)
    {
        (AddTag(std::forward<Args>(args)),...);
    }
    
public:
    const FTransform& GetWorldTransform() const;
    void SetWorldTransform(const FTransform& trans);
    void SetWorldTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot);
    
    const FVector3D& GetWorldScale() const;
    void SetWorldScale(const FVector3D& scale);
    void SetWorldScale(float x, float y, float z);
    void SetWorldScale(const FVector2D& scale);
    void SetWorldScale(float x, float y);
    const FVector3D& AddWorldScale(const FVector3D& scale);
    const FVector3D& AddWorldScale(float x, float y, float z);
    const FVector3D& AddWorldScale(const FVector2D& scale);
    const FVector3D& AddWorldScale(float x, float y);
    const FVector3D& AddWorldScale(float value);
    const FVector3D& AddWorldScaleX(float x);
    const FVector3D& AddWorldScaleY(float y);
    
    const FVector3D& GetWorldPosition() const;
    void SetWorldPosition(const FVector3D& pos);
    void SetWorldPosition(float x, float y, float z);
    void SetWorldPosition(const FVector2D& pos);
    void SetWorldPosition(float x, float y);
    const FVector3D& AddWorldPosition(const FVector3D& pos);
    const FVector3D& AddWorldPosition(float x, float y, float z);
    const FVector3D& AddWorldPosition(const FVector2D& pos);
    const FVector3D& AddWorldPosition(float x, float y);
    const FVector3D& AddWorldPosition(float value);
    const FVector3D& AddWorldPositionX(float x);
    const FVector3D& AddWorldPositionY(float y);
    
    const FRotator& GetWorldRotation() const;
    void SetWorldRotation(const FRotator& rot);
    void SetWorldRotation(float x, float y, float z);
    void SetWorldRotation(const FVector2D& rot);
    void SetWorldRotation(float x, float y);

    const FTransform& GetRelativeTransform() const;
    void SetRelativeTransform(const FTransform& trans);
    void SetRelativeTransform(const FVector3D& pos, const FVector3D& scale, const FRotator& rot);

    const FVector3D& GetRelativeScale() const;
    void SetRelativeScale(const FVector3D& scale);
    void SetRelativeScale(float x, float y, float z);
    void SetRelativeScale(const FVector2D& scale);
    void SetRelativeScale(float x, float y);
    const FVector3D& AddRelativeScale(const FVector3D& scale);
    const FVector3D& AddRelativeScale(float x, float y, float z);
    const FVector3D& AddRelativeScale(const FVector2D& scale);
    const FVector3D& AddRelativeScale(float x, float y);
    const FVector3D& AddRelativeScale(float value);
    const FVector3D& AddRelativeScaleX(float x);
    const FVector3D& AddRelativeScaleY(float y);
    
    const FVector3D& GetRelativePosition() const;
    void SetRelativePosition(const FVector3D& pos);
    void SetRelativePosition(float x, float y, float z);
    void SetRelativePosition(const FVector2D& pos);
    void SetRelativePosition(float x, float y);
    const FVector3D& AddRelativePosition(const FVector3D& scale);
    const FVector3D& AddRelativePosition(float x, float y, float z);
    const FVector3D& AddRelativePosition(const FVector2D& scale);
    const FVector3D& AddRelativePosition(float x, float y);
    const FVector3D& AddRelativePosition(float value);
    const FVector3D& AddRelativePositionX(float x);
    const FVector3D& AddRelativePositionY(float y);
    
    const FRotator& GetRelativeRotation() const;
    void SetRelativeRotation(const FRotator& rot);
    void SetRelativeRotation(float x, float y, float z);
    void SetRelativeRotation(const FVector2D& rot);
    void SetRelativeRotation(float x, float y);
    const FRotator& AddRelativeRotation(const FVector3D& scale);
    const FRotator& AddRelativeRotation(float x, float y, float z);
    const FRotator& AddRelativeRotation(const FVector2D& scale);
    const FRotator& AddRelativeRotation(float x, float y);
    const FRotator& AddRelativeRotation(float value);
    const FRotator& AddRelativeRotationX(float x);
    const FRotator& AddRelativeRotationY(float y);
    
public:
    template<typename T>
    Ptr<T> FindComponent(const std::string& name) const
    {
        Ptr<SceneComponent> foundComp = FindSceneComponent(name);
        if (foundComp)
            return Cast<SceneComponent, T>(foundComp);

        Ptr<ActorComponent> foundActorComp = FindActorComponent(name);
        if (foundActorComp)
            return Cast<ActorComponent, T>(foundActorComp);

        return nullptr;
    }

    template<typename T>
    Ptr<T> FindSceneComponent(const std::string& name) const
    {
        Ptr<SceneComponent> foundComp = FindSceneComponent(name);
        if (foundComp)
            return Cast<SceneComponent, T>(foundComp);

        return nullptr;
    }

    template<typename T>
    Ptr<T> FindActorComponent(const std::string& name) const
    {
        Ptr<ActorComponent> foundActorComp = FindActorComponent(name);
        if (foundActorComp)
            return Cast<ActorComponent, T>(foundActorComp);

        return nullptr;
    }

protected:

    template<typename T>
    Ptr<T> CreateSceneComponent(const std::string& name)
    {
        Ptr<T> comp = New<T>();
        if (!comp->Init(_componentID, name, This<Actor>()))
        {
            comp->Destroy();
            Delete(comp);
        }

        _componentFinder[name] = _componentID;
        _componentID++;

        return comp;
    }

    template<typename T>
    Ptr<T> CreateActorComponent(const std::string& name)
    {
        Ptr<T> comp = New<T>();
        if (!comp->Init(_componentID, name, This<Actor>()))
        {
            comp->Destroy();
            Delete(comp);
        }

        _componentFinder[name] = _componentID;
        _actorComponents[_componentID] = comp;
        _componentID++;

        return comp;
    }
};
