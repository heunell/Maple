#include "pch.h"
#include "CameraZone.h"
#include "Level.h"
#include "Component/CameraComponent.h"
#include "Component/AABBCollisionComponent.h"

bool CameraZone::Init(int32 Id, const FVector3D& Position, const FVector3D& Scale, const FRotator& Rotator, const std::string& Name)
{
    if(!Actor::Init(Id, Position, Scale, Rotator, Name))
    {
        return false;
    }

    _LockTrigger = CreateSceneComponent<AABBCollisionComponent>("CameraZoneTrigger");

    _LockTrigger->SetCollisionProfile("CameraZone");

    _LockTrigger->SetCollisionCallBack(eCollisionState::COLLISION_STATE_OVERLAP, this, &CameraZone::OnTriggerZone);
    
    _LockTrigger->AttachToComponent(GetRoot());
    
    return true;
}

void CameraZone::SetTriggerSize(float X, float Y)
{
    _LockTrigger->SetBoxSize(X, Y);
}

void CameraZone::OnTriggerZone(Weak<class CollisionComponent> Component)
{
    Ptr<Level> Level = GetLevel();

    if(!Level)
    {
        return;
    }

    Ptr<CameraComponent> Camera = Level->GetMainCamera();

    if(!Camera)
    {
        return;
    }

    const FAABB2D& LockBound = _LockTrigger->GetBox();

    Camera->SetCameraLock(LockBound);
}
