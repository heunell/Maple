#pragma once
#include "Core/Object.h"
#include "Common/IArchive.h"
class Component : public Object, public IArchive
{
public:
    Component();
    
    virtual ~Component();
    
protected:
    Weak<class Level> _Level;
    
    Weak<class Actor> _Owner;
    
    std::string _Name;
    
    int32 _Id = -1;
    
    COMPONENT_TYPE::Type _Type = COMPONENT_TYPE::END;
    
public:
    const std::string& GetName() const            { return _Name; }
                                                  
    void SetName(const std::string& Name)         { _Name = Name; }
                                                  
    const int32 GetComponentID() const            { return _Id;   }
                                                  
    void SetComponentID(int32 id)                 { _Id = id;     }
    
    const Ptr<class Actor> GetOwner() const;
    
    void SetOwner(Ptr<class Actor> owner);
    
    Ptr<class Level> GetLevel() const;
    
    COMPONENT_TYPE::Type GetComponentType() const { return _Type; }
    
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner);
    
    virtual void Tick(float DeltaTime);
    
    virtual void Collision(float DeltaTime);
    
    virtual void Render(float DeltaTime);
    
    virtual void Destroy() override;
    
public:
    virtual void Save(std::ofstream& file);
    
    virtual void Load(std::ifstream& file);
};
