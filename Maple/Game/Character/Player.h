#pragma once
#include "Object/Pawn.h"

class Player : public Pawn
{
public:
    Player() = default;
    virtual ~Player() = default;
    
private:
    Ptr<class MovementComponent> _Movement;

    Ptr<class CameraComponent> _Camera;

    Ptr<class AABBCollisionComponent> _AABBCollision;

    int32 _TimerID = -1;
    
    bool _bReverse = false;
    
    float _Opacity = 0.f;
    
public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void Collision(float DeltaTime) override;
    
    virtual void Render(float DeltaTime) override;
    
    virtual void Destroy() override;
    
    bool IsRight() const;

private:
    void MoveRight(float DeltaTime);
    
    void MoveLeft (float DeltaTime);
    
    void MoveUp   (float DeltaTime);
    
    void MoveDown (float DeltaTime);
    
    void MoveStop (float DeltaTime);
    
    void Jump     (float DeltaTime);
    
    void Prone    (float DeltaTime);

    void OnGround (Weak<class CollisionComponent> Collision);

    void OnLeftWall (Weak<class CollisionComponent> Collision);
    
    void OnRightWall (Weak<class CollisionComponent> Collision);
};
