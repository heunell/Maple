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
    std::string _Name;
   
    int32 _ComponentID = 0;
    
    int32 _Id = -1;
    
    std::set<std::string> _tags;
    
    eActorType _Type = eActorType::Actor;
    
protected:
    Ptr<class SceneComponent> _Root;
    
    Weak<class Level> _Level;
    
    std::map<int, Ptr<class ActorComponent>> _ActorComponents;
    
    std::unordered_map<std::string, int> _ComponentFinder;
    
public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name);
    
    virtual void Tick(float DeltaTime);
    
    virtual void Collision(float DeltaTime);
    
    virtual void Render(float DeltaTime);
    
    virtual void Destroy() override;

    virtual void Save(std::ofstream& File);

    virtual void Load(std::ifstream& File);
    
public:
    Ptr<class SceneComponent> GetRoot() const;
   
    void SetRootComponent(const Ptr<class SceneComponent> comp);
    
    Ptr<class SceneComponent> FindSceneComponent(const std::string& Name) const;
    
    Ptr<class ActorComponent> FindActorComponent(const std::string& Name) const;
    
    Ptr<class SceneComponent> FindSceneComponent(int32 Id) const;
    
    Ptr<class ActorComponent> FindActorComponent(int32 Id) const;
    
    Ptr<class Level> GetLevel() const;
    
    void SetLevel(Ptr<class Level> level);
    
public:
    void Remove();
   
    void SetName(const std::string& Name);
    
    const std::string& GetName() const;
    
    eActorType GetType() const;
    
public:
    const int32 GetActorID() const { return _Id; }
    
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
    
    void SetWorldTransform(const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator);
    
    const FVector3D& GetWorldScale() const;
    
    void SetWorldScale(const FVector3D& Scale);
    
    void SetWorldScale(float x, float y, float z);
    
    void SetWorldScale(const FVector2D& Scale);
    
    void SetWorldScale(float x, float y);
    
    const FVector3D& AddWorldScale(const FVector3D& Scale);
    
    const FVector3D& AddWorldScale(float x, float y, float z);
    
    const FVector3D& AddWorldScale(const FVector2D& Scale);
    
    const FVector3D& AddWorldScale(float x, float y);
    
    const FVector3D& AddWorldScale(float value);
    
    const FVector3D& AddWorldScaleX(float x);
    
    const FVector3D& AddWorldScaleY(float y);
    
    const FVector3D& GetWorldPosition() const;
    
    void SetWorldPosition(const FVector3D& Position);
    
    void SetWorldPosition(float x, float y, float z);
    
    void SetWorldPosition(const FVector2D& Position);
    
    void SetWorldPosition(float x, float y);
    
    const FVector3D& AddWorldPosition(const FVector3D& Position);
    
    const FVector3D& AddWorldPosition(float x, float y, float z);
    
    const FVector3D& AddWorldPosition(const FVector2D& Position);
    
    const FVector3D& AddWorldPosition(float x, float y);
    
    const FVector3D& AddWorldPosition(float value);
    
    const FVector3D& AddWorldPositionX(float x);
    
    const FVector3D& AddWorldPositionY(float y);
    
    const FRotator& GetWorldRotation() const;
    
    void SetWorldRotation(const FRotator& Rotator);
    
    void SetWorldRotation(float x, float y, float z);
    
    void SetWorldRotation(const FVector2D& Rotator);
    
    void SetWorldRotation(float x, float y);

    const FTransform& GetRelativeTransform() const;

    void SetRelativeTransform(const FTransform& trans);

    void SetRelativeTransform(const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator);

    const FVector3D& GetRelativeScale() const;

    void SetRelativeScale(const FVector3D& Scale);

    void SetRelativeScale(float x, float y, float z);

    void SetRelativeScale(const FVector2D& Scale);

    void SetRelativeScale(float x, float y);

    const FVector3D& AddRelativeScale(const FVector3D& Scale);

    const FVector3D& AddRelativeScale(float x, float y, float z);

    const FVector3D& AddRelativeScale(const FVector2D& Scale);

    const FVector3D& AddRelativeScale(float x, float y);

    const FVector3D& AddRelativeScale(float value);

    const FVector3D& AddRelativeScaleX(float x);

    const FVector3D& AddRelativeScaleY(float y);

    const FVector3D& GetRelativePosition() const;

    void SetRelativePosition(const FVector3D& Position);

    void SetRelativePosition(float x, float y, float z);

    void SetRelativePosition(const FVector2D& Position);

    void SetRelativePosition(float x, float y);

    const FVector3D& AddRelativePosition(const FVector3D& Scale);

    const FVector3D& AddRelativePosition(float x, float y, float z);

    const FVector3D& AddRelativePosition(const FVector2D& Scale);

    const FVector3D& AddRelativePosition(float x, float y);

    const FVector3D& AddRelativePosition(float value);

    const FVector3D& AddRelativePositionX(float x);

    const FVector3D& AddRelativePositionY(float y);

    const FRotator& GetRelativeRotation() const;

    void SetRelativeRotation(const FRotator& Rotator);

    void SetRelativeRotation(float x, float y, float z);

    void SetRelativeRotation(const FVector2D& Rotator);

    void SetRelativeRotation(float x, float y);

    const FRotator& AddRelativeRotation(const FVector3D& Scale);

    const FRotator& AddRelativeRotation(float x, float y, float z);

    const FRotator& AddRelativeRotation(const FVector2D& Scale);

    const FRotator& AddRelativeRotation(float x, float y);

    const FRotator& AddRelativeRotation(float value);

    const FRotator& AddRelativeRotationX(float x);

    const FRotator& AddRelativeRotationY(float y);

public:
    template<typename T>
    Ptr<T> FindComponent(const std::string& Name) const
    {
        Ptr<SceneComponent> FoundComponent = FindSceneComponent(Name);

        if (FoundComponent)
        {
            return Cast<SceneComponent, T>(FoundComponent);
        }

        Ptr<ActorComponent> FoundActorComponent = FindActorComponent(Name);

        if (FoundActorComponent)
        {
            return Cast<ActorComponent, T>(FoundActorComponent);
        }

        return nullptr;
    }

    template<typename T>
    Ptr<T> FindSceneComponent(const std::string& Name) const
    {
        Ptr<SceneComponent> FoundComponent = FindSceneComponent(Name);

        if (FoundComponent)
        {
            return Cast<SceneComponent, T>(FoundComponent);
        }

        return nullptr;
    }

    template<typename T>
    Ptr<T> FindActorComponent(const std::string& Name) const
    {
        Ptr<ActorComponent> FoundActorComponent = FindActorComponent(Name);

        if (FoundActorComponent)
        {
            return Cast<ActorComponent, T>(FoundActorComponent);
        }

        return nullptr;
    }

protected:

    template<typename T>
    Ptr<T> CreateSceneComponent(const std::string& Name)
    {
        Ptr<T> Component = New<T>();

        if (!Component->Init(_ComponentID, Name, This<Actor>()))
        {
            Component->Destroy();

            Delete(Component);
        }

        _ComponentFinder[Name] = _ComponentID;
        
        _ComponentID++;

        return Component;
    }

    template<typename T>
    Ptr<T> CreateActorComponent(const std::string& Name)
    {
        Ptr<T> Component = New<T>();
        if (!Component->Init(_ComponentID, Name, This<Actor>()))
        {
            Component->Destroy();

            Delete(Component);
        }

        _ComponentFinder[Name] = _ComponentID;
        
        _ActorComponents[_ComponentID] = Component;
        
        _ComponentID++;

        return Component;
    }
};
