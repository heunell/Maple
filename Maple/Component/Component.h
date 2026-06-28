#pragma once
#include "Core/Object.h"
#include "Common/IArchive.h"
class Component : public Object, public IArchive
{
public:
    Component();
    virtual ~Component();
    
protected:
    Weak<class Level> _level;
    Weak<class Actor> _owner;
    std::string _name;
    int32 _id = -1;
    COMPONENT_TYPE::Type _type = COMPONENT_TYPE::END;
    
public:
    const std::string& GetName() const { return _name; }
    void SetName(const std::string& name) { _name = name; }
    const int32 GetComponentID() const { return _id; }
    void SetComponentID(int32 id) { _id = id; }
    const Ptr<class Actor> GetOwner() const;
    void SetOwner(Ptr<class Actor> owner);
    Ptr<class Level> GetLevel() const;
    COMPONENT_TYPE::Type GetComponentType() const { return _type; }
    
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner);
    virtual void Tick(float deltaTime);
    virtual void Collision(float deltaTime);
    virtual void Render(float deltaTime);
    virtual void Destroy() override;
    
public:
    virtual void Save(std::ofstream& file);
    virtual void Load(std::ifstream& file);
};
