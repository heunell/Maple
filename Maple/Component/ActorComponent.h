#pragma once
#include "Component.h"

class ActorComponent : public Component
{
public:
    ActorComponent();
    virtual ~ActorComponent();
    
public:
    virtual bool Init(int32 id, const std::string& name, Ptr<class Actor> owner) override;
    virtual void Tick(float deltaTime);
    virtual void Destroy() override;
    virtual void Save(std::ofstream& file);
    virtual void Load(std::ifstream& file);
};
