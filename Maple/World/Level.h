#pragma once
#include "Core/Object.h"
#include "Common/IArchive.h"
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
    int32 _actorID = 0;
    std::map<int32, Ptr<class Actor>> _actors;
    std::vector<int32> _removeActors;
    
public:
    virtual bool Init(const std::string& path);
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void RenderUI(float deltaTime);
    virtual void Destory() override;
    virtual void Save(std::ofstream& file);
    virtual void Load(std::ifstream& file);
    virtual void AddTag(const std::string& tag, int32 id);
    void DeleteTag(Ptr<class Actor> actor);
    const std::map<int32, Ptr<class Actor>>& GetActors() const;
    
public:
    template<typename T>
    Ptr<T> FindActor(int32 id)
    {
        auto it = _actors.find(id);
        if (_actors.end() == it)
            return nullptr;

        return Cast<Actor, T>(it->second);
    }

    template<typename T>
    Ptr<T> SpawnActor(const std::string& name, const FVector3D& pos, const FVector3D& scale, const FRotator& rot)
    {
        Ptr<T> actor = New<T>();

        actor->SetLevel(This<Level>());

        int32 actorID = _actorID++;

        if (!actor->Init(actorID, pos, scale, rot, name))
        {
            DESTROY(actor)
                return nullptr;
        }

        _actors[actorID] = actor;

        return actor;
    }
};
