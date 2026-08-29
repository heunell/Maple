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

    if(!_LockTrigger)
    {
        return false;
    }

    _LockTrigger->SetCollisionProfile("CameraZone");

    _LockTrigger->SetCollisionCallBack(eCollisionState::COLLISION_STATE_OVERLAP, this, &CameraZone::OnTriggerZone);
    
    _LockTrigger->AttachToComponent(GetRoot());
    
    return true;
}

void CameraZone::SetArea(float X, float Y)
{
    _LockType = eCameraZoneType::Area;

    _LockTrigger->SetBoxSize(X, Y);
}

void CameraZone::SetWall(float X, float Y, const FAABB2D &LockBound)
{
    _LockType = eCameraZoneType::Wall;

    _LockTrigger->SetBoxSize(X, Y);

    _WallLockBound = LockBound;
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

    if (_LockType == eCameraZoneType::Area)
    {
        Camera->SetCameraLock(_LockTrigger->GetBox());

        return;
    }

    if (_LockType == eCameraZoneType::Wall)
    {
        Camera->SetCameraLock(_WallLockBound);
    }
}
