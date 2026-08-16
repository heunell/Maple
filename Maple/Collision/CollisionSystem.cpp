#include "pch.h"
#include "CollisionSystem.h"
#include "Component/AABBCollisionComponent.h"
#include "Component/SphereCollisionComponent.h"
#include "Component/OBBCollisionComponent.h"

bool CollisionSystem::AABBToAABB(Weak<class AABBCollisionComponent> Source, Weak<class AABBCollisionComponent> Destination)
{
    Ptr<AABBCollisionComponent> SourceCollision = Lock<AABBCollisionComponent>(Source);

    Ptr<AABBCollisionComponent> DestinationCollision = Lock<AABBCollisionComponent>(Destination);

    if (!SourceCollision || !DestinationCollision)
    {
        return false;
    }

    return AABBToAABB(SourceCollision->GetBox(), DestinationCollision->GetBox());
}

bool CollisionSystem::AABBToAABB(const FAABB2D& Source, const FAABB2D& Destination)
{
    if (Source._Min._x > Destination._Max._x)
    {
        return false;
    }
    
    else if (Destination._Min._x > Source._Max._x)
    {
        return false;
    }
    else if (Source._Min._y > Destination._Max._y)
    {
        return false;
    }
    else if (Destination._Min._y > Source._Max._y)
    {
        return false;
    }

    return true;
}

bool CollisionSystem::AABBToSphere(Weak<class AABBCollisionComponent> Source, Weak<class SphereCollisionComponent> Destination)
{
    Ptr<AABBCollisionComponent> SourceCollision = Lock<AABBCollisionComponent>(Source);
    
    Ptr<SphereCollisionComponent> DestinationCollision = Lock<SphereCollisionComponent>(Destination);

    if (!SourceCollision || !DestinationCollision)
    {
        return false;
    }

    return AABBToSphere(SourceCollision->GetBox(), DestinationCollision->GetWorldPosition(), DestinationCollision->GetRadius()); // ShpereCollisionComponent 가 없어서 
}

bool CollisionSystem::AABBToSphere(const FAABB2D& Source, const FVector3D& DestinationCenter, float DestinationRadius)
{
    float FoundX = Utility::Clamp<float>(DestinationCenter._x, Source._Min._x, Source._Max._x);

    float FoundY = Utility::Clamp<float>(DestinationCenter._y, Source._Min._y, Source._Max._y);
    
    float DistanceX = DestinationCenter._x - FoundX;
    
    float DistanceY = DestinationCenter._y - FoundY;

    float FoundDistanceSquare = (DistanceX * DistanceX) + (DistanceY * DistanceY);

    float RadiusSquare = (DestinationRadius * DestinationRadius);

    if (FoundDistanceSquare > RadiusSquare)
    {
        return false;
    }

    return true;

}

bool CollisionSystem::AABBToOBB(Weak<class AABBCollisionComponent> Source, Weak<class OBBCollisionComponent> Destination)
{
    Ptr<AABBCollisionComponent> SourceCollision = Lock<AABBCollisionComponent>(Source);

    Ptr<OBBCollisionComponent> DestinationCollision = Lock<OBBCollisionComponent>(Destination);

    if (!SourceCollision || !DestinationCollision)
    {
        return false;
    }
    
    return AABBToOBB(SourceCollision->GetBox(), DestinationCollision->GetBox());    // OBBCollisionComponent 가 없어서 
}

bool CollisionSystem::AABBToOBB(const FAABB2D& SourceAABB, const FOBB2D& DestinationOBB)
{
    FOBB2D OBB = CreateOBB(SourceAABB);

    return OBBToOBB(OBB, DestinationOBB);
}

bool CollisionSystem::SphereToSphere(Weak<class SphereCollisionComponent> Source, Weak<class SphereCollisionComponent> Destination)
{
    Ptr<SphereCollisionComponent> SourceCollision = Lock<SphereCollisionComponent>(Source);

    Ptr<SphereCollisionComponent> DestinationCollision = Lock<SphereCollisionComponent>(Destination);

    if (!SourceCollision || !DestinationCollision)
        return false;

    return SphereToSphere(SourceCollision->GetWorldPosition(), SourceCollision->GetRadius(), DestinationCollision->GetWorldPosition(), DestinationCollision->GetRadius());
    // SphereCollisionComponent가 없어서
}

bool CollisionSystem::SphereToSphere(const FVector3D& SourceCenter, float SourceRadiuse, const FVector3D& DestinationCenter, float DestinationRadius)
{
    float Distance = SourceCenter.Distance(DestinationCenter);

    if (Distance > SourceRadiuse + DestinationRadius)
    {
        return false;
    }

    return true;
}

bool CollisionSystem::SphereToOBB(Weak<class SphereCollisionComponent> Source, Weak<class OBBCollisionComponent> Destination)
{
    Ptr<SphereCollisionComponent> SourceCollision = Lock<SphereCollisionComponent>(Source);

    Ptr<OBBCollisionComponent> DestinationCollision = Lock<OBBCollisionComponent>(Destination);

    if (!SourceCollision || !DestinationCollision)
    {
        return false;
    }

    return SphereToOBB(SourceCollision->GetWorldPosition(), SourceCollision->GetRadius(), DestinationCollision->GetBox());
}

bool CollisionSystem::SphereToOBB(const FVector3D& SourceCenter, float SourceRadius, const FOBB2D& DestinationOBB)
{
    FVector2D ConvertCenter;

    ConvertCenter._x = SourceCenter._x;

    ConvertCenter._y = SourceCenter._y;

    FVector2D CenterLine = ConvertCenter - DestinationOBB._center;

    FVector2D Axis = CenterLine;

    Axis.Normalize();

    float CenterProjection = std::abs(CenterLine.Dot(Axis));

    float OBBProjectionOnAxis = std::abs(Axis.Dot(DestinationOBB._Axis[AXIS_TYPE::X])) * DestinationOBB._halfSize._x + std::abs(Axis.Dot(DestinationOBB._Axis[AXIS_TYPE::Y])) * DestinationOBB._halfSize._y;
    
    if (CenterProjection > SourceRadius + OBBProjectionOnAxis)
    {
        return false;
    }

    float ProjectionX = std::abs(CenterLine.Dot(DestinationOBB._Axis[AXIS_TYPE::X]));

    if (ProjectionX > SourceRadius + DestinationOBB._halfSize._x)
    {
        return false;
    }

    float ProjectionY = std::abs(CenterLine.Dot(DestinationOBB._Axis[AXIS_TYPE::Y]));
    
    return true;
}

bool CollisionSystem::OBBToOBB(Weak<class OBBCollisionComponent> Source, Weak<class OBBCollisionComponent> Destination)
{

    Ptr<OBBCollisionComponent> SourceCollision = Lock<OBBCollisionComponent>(Source);
    Ptr<OBBCollisionComponent> DestinationCollision = Lock<OBBCollisionComponent>(Destination);

    if (!SourceCollision || !DestinationCollision)
        return false;


    return OBBToOBB(SourceCollision->GetBox(), DestinationCollision->GetBox());
}

bool CollisionSystem::OBBToOBB(const FOBB2D& Source, const FOBB2D& Destination)
{
    FVector2D CenterLine = Source._center - Destination._center;

    if (!ComputeAxisProjection(CenterLine, Source._Axis[AXIS_TYPE::X], Source._halfSize._x, Destination._Axis, Destination._halfSize))
        return false;

    if (!ComputeAxisProjection(CenterLine, Source._Axis[AXIS_TYPE::Y], Source._halfSize._y, Destination._Axis, Destination._halfSize))
        return false;

    if (!ComputeAxisProjection(CenterLine, Destination._Axis[AXIS_TYPE::X], Destination._halfSize._x, Source._Axis, Source._halfSize))
        return false;

    if (!ComputeAxisProjection(CenterLine, Destination._Axis[AXIS_TYPE::Y], Destination._halfSize._y, Source._Axis, Source._halfSize))
        return false;

    return true;
}

bool CollisionSystem::ComputeAxisProjection(const FVector2D& CenterLine, const FVector2D& SourceAxis, float AxisHalfSize, const FVector2D* DestinationAxis, const FVector2D& DestinationHalfSize)
{
    float CenterProjectionDistance = std::abs(CenterLine.Dot(SourceAxis));

    float DestinationProjectionDistance = std::abs(SourceAxis.Dot(DestinationAxis[AXIS_TYPE::X])) * DestinationHalfSize._x + std::abs(SourceAxis.Dot(DestinationAxis[AXIS_TYPE::Y])) * DestinationHalfSize._y;

    float overlap = AxisHalfSize + DestinationProjectionDistance - CenterProjectionDistance;
    
    if (overlap <= 0)
    {
        return false;
    }

    return true;
}

FOBB2D CollisionSystem::CreateOBB(const FAABB2D& AABB)
{
    FOBB2D ReturnValue;

    ReturnValue._center = (AABB._Min + AABB._Max) * 0.5f;
    
    ReturnValue._Axis[AXIS_TYPE::X]._x = 1.f;
    
    ReturnValue._Axis[AXIS_TYPE::X]._y = 0.f;
    
    ReturnValue._Axis[AXIS_TYPE::Y]._x = 0.f;
    
    ReturnValue._Axis[AXIS_TYPE::Y]._y = 1.f;
    
    ReturnValue._halfSize = (AABB._Max - AABB._Min) * 0.5f;

    return ReturnValue;
}
