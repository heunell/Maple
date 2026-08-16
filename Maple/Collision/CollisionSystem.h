#pragma once

#include "Common/Info.h"
#include "Core/Defines.h"

class CollisionSystem
{
public:
    //= AABB =
    static bool AABBToAABB(Weak<class AABBCollisionComponent> Source, Weak<class AABBCollisionComponent> Destination);
    
    static bool AABBToAABB(const FAABB2D& Source, const FAABB2D& Destination);

    static bool AABBToSphere(Weak<class AABBCollisionComponent> Source, Weak<class SphereCollisionComponent> Destination);
    
    static bool AABBToSphere(const FAABB2D& Source, const FVector3D& DestinationCenter, float DestinationRadius);

    static bool AABBToOBB(Weak<class AABBCollisionComponent> Source, Weak<class OBBCollisionComponent> Destination);
    
    static bool AABBToOBB(const FAABB2D& SourceAABB, const FOBB2D& DestinationOBB);

    //= Sphere =
    static bool SphereToSphere(Weak<class SphereCollisionComponent> Source, Weak<class SphereCollisionComponent> Destination);
    
    static bool SphereToSphere(const FVector3D& SourceCenter, float SourceRadiuse, const FVector3D& DestinationCenter, float DestinationRadius);

    static bool SphereToOBB(Weak<class SphereCollisionComponent> Source, Weak<class OBBCollisionComponent> Destination);
    
    static bool SphereToOBB(const FVector3D& SourceCenter, float SourceRadius, const FOBB2D& DestinationOBB);


    //= OBB =
    static bool OBBToOBB(Weak<class OBBCollisionComponent> Source, Weak<class OBBCollisionComponent> Destination);
    
    static bool OBBToOBB(const FOBB2D& Source, const FOBB2D& Destination);


private:
    //static float Clamp(float value, float minValue, float maxValue);

    static bool ComputeAxisProjection(const FVector2D& CenterLine, const FVector2D& SourceAxis, float AxisHalfSize, const FVector2D* DestinationAxis, const FVector2D& DestinationHalfSize);

    static FOBB2D CreateOBB(const FAABB2D& AABB);
};

