#pragma once
#include "Object/Actor.h"

class Controller : public Actor
{
public:
    Controller() = default;
    virtual ~Controller() = default;
    Controller(const Controller&) = delete;
    Controller(Controller&&) = delete;
    Controller& operator=(const Controller&) = delete;
    Controller& operator=(Controller&&) = delete;
    
private:
    Weak<class Pawn> _pawn;
    
public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;
    
public:
    void SetPawn(Ptr<class Pawn> pawn);
    Ptr<class Pawn> GetPawn() const;
};
