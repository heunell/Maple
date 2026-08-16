#pragma once
#include "Object/Actor.h"
#include "Controller/Controller.h"

class Pawn : public Actor
{
public:
    Pawn();
    virtual ~Pawn();
    
protected:
    Ptr<Controller> _Controller;
    
public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void Collision(float DeltaTime) override;
    
    virtual void Render(float DeltaTime) override;
    
    virtual void Destroy() override;
    
public:
    void SetController(Ptr<Controller> Controll);
    
    template<typename T>
    Ptr<T> GetController() const
    {
        return Cast<Controller, T>(_Controller);
    }
};
