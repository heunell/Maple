#include "pch.h"
#include "AABBCollisionComponent.h"
#include "SphereCollisionComponent.h"
#include "OBBCollisionComponent.h"
#include "Collision/CollisionSystem.h"
#include "Core/AssetManager.h"
#include "Core/MeshManager.h"
#include "Core/Mesh.h"

AABBCollisionComponent::AABBCollisionComponent()
{
}

AABBCollisionComponent::~AABBCollisionComponent()
{
}

const FVector2D& AABBCollisionComponent::GetBoxSize() const
{
    return _BoxSize;
}

const FAABB2D& AABBCollisionComponent::GetBox() const
{
    return _Box;
}

void AABBCollisionComponent::SetBoxSize(const FVector2D& Size)
{
    SetBoxSize(Size._x, Size._y);
}

void AABBCollisionComponent::SetBoxSize(float X, float Y)
{
    _BoxSize._x = X;

    _BoxSize._y = Y;
}

bool AABBCollisionComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    CollisionComponent::Init(Id, Name, Owner);

#if _DEBUG
    _Mesh = MESH_MANAGER->FindMesh("FrameRect");
#endif 

    _Type = COMPONENT_TYPE::AABB;

    return true;
}

void AABBCollisionComponent::Tick(float DeltaTime)
{
    CollisionComponent::Tick(DeltaTime);

    _Box._Min._x = _World._position._x - _BoxSize._x * 0.5f;
          
    _Box._Max._x = _World._position._x - _BoxSize._x * 0.5f;
          
    _Box._Min._y = _World._position._y - _BoxSize._y * 0.5f;
          
    _Box._Max._y = _World._position._y - _BoxSize._y * 0.5f;

    _Min._x = _Box._Min._x;

    _Max._x = _Box._Max._x;

    _Min._y = _Box._Min._y;

    _Max._y = _Box._Max._y;

    SetWorldScale(_BoxSize);
}

void AABBCollisionComponent::Collision(float DeltaTime)
{
    CollisionComponent::Collision(DeltaTime);
}

void AABBCollisionComponent::Render(float DeltaTime)
{
    CollisionComponent::Render(DeltaTime);
}

void AABBCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}

bool AABBCollisionComponent::Collision(Weak<CollisionComponent> Destination)
{
    Ptr<CollisionComponent> DestinationCollision = Lock<CollisionComponent>(Destination);

    if (nullptr == DestinationCollision)
    {
        return false;
    }

    switch (DestinationCollision->GetShape())
    {
    case eCollisionShape::AABB:
        return CollisionSystem::AABBToAABB(This<AABBCollisionComponent>(), Cast<CollisionComponent, AABBCollisionComponent>(DestinationCollision));
    case eCollisionShape::Sphere:
        return CollisionSystem::AABBToSphere(This<AABBCollisionComponent>(), Cast<CollisionComponent, SphereCollisionComponent>(DestinationCollision));
    case eCollisionShape::OBB:
        return CollisionSystem::AABBToOBB(This<AABBCollisionComponent>(), Cast<CollisionComponent, OBBCollisionComponent>(DestinationCollision));
    default:
        break;
    }

    return false;
}
