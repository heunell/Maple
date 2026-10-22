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

    Weak<class CollisionComponent> _CurrentGround;

    Weak<class CollisionComponent> _IgnoredGround;

    int32 _TimerID = -1;
    
    bool _bReverse = false;
    
    float _Opacity = 0.f;
    
    int32 _MaxHP = 50000;
    
    int32 _CurrentHP = 50000;
    
    int32 _MaxMP = 30000;
    
    int32 _CurrentMP = 30000;
    
    float _InvincTimer = 0.f;
    
    float _ReviveInvincibleTimer = 0.f;
    
public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name) override;
    
    virtual void Tick(float DeltaTime) override;
    
    virtual void Collision(float DeltaTime) override;
    
    virtual void Render(float DeltaTime) override;
    
    virtual void Destroy() override;
    
    bool IsRight() const;

    void MoveRight();

    void MoveLeft();

    void MoveStop();

    bool Jump();

    bool DoubleJump();

    bool UpJump();

    bool DownJump();

    void TakeDamage(int32 Damage, bool IgnoreInvincible = false);
    
    void TakePercentDamage(int32 Percent, bool IgnoreInvincible = false);
    
    void ShowDamage(int32 Damage, const FVector3D& Position);
    
    bool Revive();
    
    bool UseMP(int32 Cost);
    
    bool CanUseMP(int32 Cost) const;
    
    int32 GetHP() const;
    
    int32 GetMP() const;
    
    int32 GetMaxHP() const;
    
    int32 GetMaxMP() const;
    
private:
    void MoveUp   (float DeltaTime);
    
    void Prone    (float DeltaTime);

    void OnGround (Weak<class CollisionComponent> Collision);

    void OnLeftWall (Weak<class CollisionComponent> Collision);
    
    void OnRightWall (Weak<class CollisionComponent> Collision);

    void OnGroundRelease(Weak<class CollisionComponent> Collision);
};
