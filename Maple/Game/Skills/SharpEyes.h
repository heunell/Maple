#pragma once

#include "Skill.h"

class SharpEyes : public Skill
{
public:
    SharpEyes() = default;
    virtual ~SharpEyes() = default;

public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator Rotator, const std::string& Name, Ptr<Actor> Owner) override;
   
    virtual void Collision(float DeltaTime) override;
    
    virtual void Start() override;
    
    virtual void Update(float DeltaTime) override;
    
    virtual void End() override;
    
    virtual void Destroy() override;
};