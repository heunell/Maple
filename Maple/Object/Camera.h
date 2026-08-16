#pragma once
#include "Actor.h"

class Camera : public Actor
{
public:
    Camera();
    virtual ~Camera();
private:
    Ptr<class CameraComponent> _Camera;
public:
    virtual bool Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name);
    
    virtual void Tick(float DeltaTime);
    
    virtual void Destroy() override;
};
