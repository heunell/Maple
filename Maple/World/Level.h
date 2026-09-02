#pragma once
#include "Core/Object.h"
#include "Common/IArchive.h"
#include "Object/Actor.h"
#include <map>
#include <vector>

class Level : public Object, public IArchive
{
public:
    Level();
    virtual ~Level();
    Level(const Level&) = delete;
    Level(Level&&) = delete;
    Level& operator=(const Level&) = delete;
    Level& operator=(Level&&) = delete;
    
private:
    int32 _ActorID = 0;
   
    std::map<int32, Ptr<class Actor>> _Actors;
    
    std::vector<int32> _RemoveActors;
    
    Ptr<class TagManager> _TagManager;
    
    Ptr<class CameraManager> _CameraManager;
    
    Ptr<class CollisionManager> _CollisionManager;

public:
    virtual bool Init(const std::string& Path);
    
    virtual void Tick(float DeltaTime);
    
    virtual void Collision(float DeltaTime);
    
    virtual void Render(float DeltaTime);
    
    virtual void RenderUI(float DeltaTime);
    
    virtual void Destroy() override;
    
    virtual void Save(std::ofstream& File);
    
    virtual void Load(std::ifstream& File);
    
    virtual void AddTag(const std::string& Tag, int32 Id);
    
    void DeleteTag(Ptr<class Actor> Actor);
    
    const std::map<int32, Ptr<class Actor>>& GetActors() const;
    
    void SetMainCamera(Ptr<class CameraComponent> Camera);
    
    Ptr<class CameraComponent> GetMainCamera() const;
    
    const FMatrix& GetViewMatrix() const;
    
    const FMatrix& GetProjectionMatrix() const;
    
    const FVector3D& GetCameraWorldPosition() const;
    
    const FMatrix& GetUIProjMatrix() const;
    
    void AddCollision(std::pair<int32, int32>& CollisionID, Ptr<class CollisionComponent> Component);
    
    void RemoveCollision(std::pair<int32, int32>& CollisionID);
    
    Ptr<class CollisionComponent> FindCollider(std::pair<int32, int32>& CollisionID);
    
    Ptr<class AABBCollisionComponent> FindPlatformBelow(const FAABB2D& PlayerBox, float MaxDistance, const Ptr<class CollisionComponent>& IgnoreGround);

    void FindActors(const std::string& Tag, OUT std::vector<Ptr<class Actor>>& OutArr);
    
    void RemoveActor(int32 Id);
    
    Ptr<Actor> FindActor(int32 Id);

public:
    template<typename T>
    Ptr<T> FindActor(int32 Id)
    {
        auto It = _Actors.find(Id);

        if (_Actors.end() == It)
        {
            return nullptr;
        }

        return Cast<Actor, T>(It->second);
    }

    template<typename T, typename...Args>
    Ptr<T> SpawnActor(const std::string& Name, const FVector3D& Posision, const FVector3D& Scale, const FRotator& Rotation, Args&&...Arguments)
    {
        Ptr<T> SpawnActor = New<T>();

        SpawnActor->SetLevel(This<Level>());

        int32 ActorID = _ActorID++;

        if (!SpawnActor->Init(ActorID, Posision, Scale, Rotation, Name, std::forward<Args>(Arguments)...))
        {
            DESTROY(SpawnActor)

            return nullptr;
        }

        _Actors[ActorID] = SpawnActor;

        return SpawnActor;
    }
};
