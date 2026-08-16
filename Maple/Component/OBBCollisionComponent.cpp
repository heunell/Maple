#include "pch.h"
#include "OBBCollisionComponent.h"
#include "AABBCollisionComponent.h"
#include "SphereCollisionComponent.h"
#include "Collision/CollisionSystem.h"
#include "Core/AssetManager.h"
#include "Core/MeshManager.h"
#include "Shader/TransformCBuffer.h"

OBBCollisionComponent::OBBCollisionComponent()
{
    _Shape = eCollisionShape::OBB;
}

OBBCollisionComponent::~OBBCollisionComponent()
{}

const FOBB2D& OBBCollisionComponent::GetBox() const
{
    return _Box;
}

const FVector2D& OBBCollisionComponent::GetBoxSize() const
{
    return _BoxSize;
}

void OBBCollisionComponent::SetBoxSize(const FVector2D& BoxSize)
{
    _BoxSize = BoxSize;
}

void OBBCollisionComponent::SetBoxSize(float X, float Y)
{
    _BoxSize._x = X;

    _BoxSize._y = Y;
}

bool OBBCollisionComponent::Init(int32 Id, const std::string& Name, Ptr<class Actor> Owner)
{
    CollisionComponent::Init(Id, Name, Owner);

#if _DEBUG

    _Mesh = MESH_MANAGER->FindMesh("FrameRect");

#endif

    _Type = COMPONENT_TYPE::OBB;

    return true;
}

void OBBCollisionComponent::Tick(float DeltaTime)
{
    CollisionComponent::Tick(DeltaTime);

    _Box._center._x = _World._position._x;
    _Box._center._y = _World._position._y;

    _Box._Axis[AXIS_TYPE::X]._x = _Axis[AXIS_TYPE::X]._x;
    _Box._Axis[AXIS_TYPE::X]._y = _Axis[AXIS_TYPE::X]._y;

    _Box._Axis[AXIS_TYPE::Y]._x = _Axis[AXIS_TYPE::Y]._x;
    _Box._Axis[AXIS_TYPE::Y]._y = _Axis[AXIS_TYPE::Y]._y;

    _Box._halfSize = _BoxSize * 0.5f;

    FVector2D pos[4];
    pos[0] = _Box._center - _Box._Axis[AXIS_TYPE::X] * _Box._halfSize._x + _Box._Axis[AXIS_TYPE::Y] * _Box._halfSize._y;
    pos[1] = _Box._center - _Box._Axis[AXIS_TYPE::X] * _Box._halfSize._x - _Box._Axis[AXIS_TYPE::Y] * _Box._halfSize._y;
    pos[2] = _Box._center + _Box._Axis[AXIS_TYPE::X] * _Box._halfSize._x + _Box._Axis[AXIS_TYPE::Y] * _Box._halfSize._y;
    pos[3] = _Box._center + _Box._Axis[AXIS_TYPE::X] * _Box._halfSize._x - _Box._Axis[AXIS_TYPE::Y] * _Box._halfSize._y;

    _Min._x = pos[0]._x;
    _Min._y = pos[0]._y;


    _Min._x = pos[0]._x;
    _Max._y = pos[0]._y;

    for (int i = 0; i < 4; ++i)
    {
        _Min._x = _Min._x > pos[i]._x ? pos[i]._x : _Min._x;
        _Min._y = _Min._y > pos[i]._y ? pos[i]._y : _Min._y;

        _Max._x = _Max._x < pos[i]._x ? pos[i]._x : _Max._x;
        _Max._y = _Max._y < pos[i]._y ? pos[i]._y : _Max._y;
    }

    SetWorldScale(_BoxSize);
}

void OBBCollisionComponent::Collision(float DeltaTime)
{
    CollisionComponent::Collision(DeltaTime);
}

bool OBBCollisionComponent::Collision(Weak<CollisionComponent> Destination)
{
    Ptr<CollisionComponent> DestinationCollision = Lock<CollisionComponent>(Destination);

    if (nullptr == DestinationCollision)
    {
        return false;
    }

    switch (DestinationCollision->GetShape())
    {
    case eCollisionShape::OBB:
        return CollisionSystem::OBBToOBB(This<OBBCollisionComponent>(), Cast<CollisionComponent, OBBCollisionComponent>(DestinationCollision));
    case eCollisionShape::AABB:
        return CollisionSystem::AABBToOBB(Cast<CollisionComponent, AABBCollisionComponent>(DestinationCollision), This<OBBCollisionComponent>());
    case eCollisionShape::Sphere:
        return  CollisionSystem::SphereToOBB(Cast<CollisionComponent, SphereCollisionComponent>(DestinationCollision), This<OBBCollisionComponent>());
    default:
        break;
    }

    return false;
}

void OBBCollisionComponent::Render(float DeltaTime)
{
    CollisionComponent::Render(DeltaTime);
}

void OBBCollisionComponent::Destroy()
{
    CollisionComponent::Destroy();
}
