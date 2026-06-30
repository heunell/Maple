#pragma once
#include "Pawn.h"

class Player : public Pawn
{
public:
    Player() = default;
    virtual ~Player() = default;
    
private:
    Ptr<class CameraComponent> _camera;
    int32 _timerID = -1;
    bool reverse = false;
    float _opacity = 0.f;
    
public:
    virtual bool Init(int32 id, const FVector3D& pos, const FVector3D& scale, const FRotator& rot, const std::string& name) override;
    virtual void Tick(float deltaTime) override;
    virtual void Collision(float deltaTime) override;
    virtual void Render(float deltaTime) override;
    virtual void Destroy() override;
    
private:
    void MoveRight(float deltaTime);
    void MoveLeft (float deltaTime);
    void MoveUp   (float deltaTime);
    void MoveDown (float deltaTime);
    void MoveStop (float deltaTime);
    void Attack   (float deltaTime);
    void Jump     (float deltaTime);
    void Prone    (float deltaTime);
};
