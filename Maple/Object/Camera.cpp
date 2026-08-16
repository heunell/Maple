#include "pch.h"
#include "Camera.h"
#include "Component/CameraComponent.h"

Camera::Camera()
{}

Camera::~Camera()
{}

bool Camera::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    Actor::Init(Id, Position, Scale, Rotator, Name);
    
    _Camera = CreateSceneComponent<CameraComponent>("camera");
    
    return true;
}

void Camera::Tick(float DeltaTime)
{
    Actor::Tick(DeltaTime);
}

void Camera::Destroy()
{}
