#pragma once
#include "Object/Actor.h"
#include "Controller/Controller.h"

class Pawn : public Actor
{
public:
    Pawn();
    virtual ~Pawn();
    
protected:
    Ptr<Controller> _controller;
    
public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;
    
public:
    void SetController(Ptr<Controller> ctrl);
    template<typename T>
    Ptr<T> GetController() const
    {
        return Cast<Controller, T>(_controller);
    }
};
