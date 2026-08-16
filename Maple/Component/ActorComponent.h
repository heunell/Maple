#pragma once
#include "Component.h"

class ActorComponent : public Component
{
public:
    ActorComponent();
    
    virtual ~ActorComponent();
    
public:
    virtual bool Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner) override;
    
    virtual void Tick(float DeltaTime);
    
    virtual void Destroy() override;
    
    virtual void Save(std::ofstream& File);
    
    virtual void Load(std::ifstream& File);
};
