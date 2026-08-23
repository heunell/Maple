#pragma once
#include "Core/Object.h"

class World : public Object
{
public:
    World();
    virtual ~World();
    World(const World&) = delete;
    World(World&&) = delete;
    World& operator=(const World&) = delete;
    World& operator=(World&&) = delete;
    
public:
    Ptr<class Level> _CurLevel;
  
    Ptr<class Level> _NextLevel;
    
    Weak<class Actor> _Player;
    
public:
    virtual void Init(const std::string& Name);
    
    virtual void Tick(float DeltaTime);
    
    virtual void Collision(float DeltaTime);
    
    virtual void Render(float DeltaTime);
    
    virtual void RenderUI(float DeltaTime);
    
    virtual void Destroy() override;
    
public:
    Ptr<class Level> GetCurLevel() const;
    
    Ptr<class Actor> GetPlayer() const;
    
    void  SetMainPlayer(Ptr<class Actor> Player);
    
public:
    template<typename T>
    Ptr<T> CreateLevel(const std::string& Path)
    {
       Ptr<T> level = New<T>();
       if (false == level->Init(Path))
       {
           DESTROY(level)

           return nullptr;
       }

       return level;
    }

private:
    
    Ptr<class CameraManager> _cameraManager;

    eLevelType _CurLevelType = eLevelType::LEVEL_END;

    eLevelType _NextLevelType = eLevelType::LEVEL_END;
    
};
